#include "server.h"
#include "utils/log.h"
#include "socketepoll.h"
#include "http/httprequest.h"
#include "yamlutil.h"
#include "signalhandler.h"
#include "timeutil.h"
#include <unistd.h>
#include <string.h>
#include <cassert>


std::string Server::ServerConfig::field_port("port");

uint16_t Server::ServerConfig::port = 0;

std::string Server::ServerConfig::field_net_thread_cnt("net_thread_cnt");

size_t Server::ServerConfig::net_thread_cnt = 0;

bool Server::ServerConfig::is_config_done = false;


Server::Server()
        : listenfd_(-1)
        , net_thread_cnt_(1)
        , running_(false) {
    
    Yaml::YamlDescriptor server_config = Yaml::Load("../framework/serverconfig.yml");
    
    if (!server_config) {
        LogE("serverconfig.yaml open failed!")
        return;
    }
    
    do {
        if (Yaml::Get(server_config, ServerConfig::field_port,
                      ServerConfig::port) < 0) {
            LogE("load port from yaml failed")
            break;
        }
        if (Yaml::Get(server_config, ServerConfig::field_net_thread_cnt,
                         (int &) ServerConfig::net_thread_cnt) < 0) {
            LogE("load net_thread_cnt from yaml failed")
            break;
        }
        if (ServerConfig::net_thread_cnt < 1) {
            ServerConfig::net_thread_cnt = 1;
        }
        if (ServerConfig::net_thread_cnt > 8) {
            ServerConfig::net_thread_cnt = 8;
        }
        net_thread_cnt_ = ServerConfig::net_thread_cnt;
        
        if (__CreateListenFd() < 0) { break; }
        
        if (__Bind(ServerConfig::port) < 0) { break; }
        
        for (int i = 0; i < net_thread_cnt_; ++i) {
            auto net_thread = new NetThread();
            net_threads_.push_back(net_thread);
        }
        
        ServerConfig::is_config_done = true;
        
    } while (false);
    
    Yaml::Close(server_config);
    
}


void Server::Serve() {
    if (!ServerConfig::is_config_done) {
        LogE("config me first!")
        assert(false);
    }
    
    SignalHandler::Instance().RegisterCallback(SIGINT, [this] {
        NotifyStop();
    });
    
    running_ = true;
    
    ::listen(listenfd_, 1024);
    
    socket_epoll_.SetListenFd(listenfd_);
    
    epoll_notifier_.SetSocketEpoll(&socket_epoll_);
    
    for (auto &net_thread : net_threads_) {
        net_thread->Start();
    }
    
    while (running_) {
        int n_events = socket_epoll_.EpollWait();
        
        if (n_events < 0) {
            int epoll_errno = socket_epoll_.GetErrNo();
            LogE("break serve, errno(%d): %s",
                 epoll_errno, strerror(epoll_errno))
            running_ = false;
            break;
        }
        
        for (int i = 0; i < n_events; ++i) {
            EpollNotifier::Notification probable_notification(
                    socket_epoll_.GetEpollDataPtr(i));
            
            if (__IsNotifyStop(probable_notification)) {
                LogI("recv notification_stop, break")
                running_ = false;
                break;
            }
            
            if (socket_epoll_.IsNewConnect(i)) {
                __OnConnect();
            }
            
            if (auto fd = (SOCKET) socket_epoll_.IsErrSet(i)) {
                __OnEpollErr(fd);
            }
        }
    }
    
    if (listenfd_ != -1) {
        LogI("close listenfd")
        ::close(listenfd_), listenfd_ = -1;
    }
    
    __NotifyNetThreadsStop();
}

void Server::NotifyStop() {
    LogI("[Server::NotifyStop]")
    running_ = false;
    epoll_notifier_.NotifyEpoll(notification_stop_);
}

Server::~Server() = default;

Server::WorkerThread::WorkerThread()
        : net_thread_(nullptr)
        , thread_seq_(__MakeWorkerThreadSeq()) {
    
}

void Server::WorkerThread::Run() {
    if (!net_thread_) {
        LogE("WorkerThread bind NetThread first!")
        running_ = false;
        return;
    }
    LogI("Launching WorkerThread %d", thread_seq_)
    
    auto recv_queue = net_thread_->GetRecvQueue();
    
    while (true) {
        
        http::RecvContext *recv_ctx;
    
//        if (recv_queue->size() > 100) {
//            net_thread_->OnWorkersHighPressure();
//        }
        
        if (recv_queue->pop_front_to(recv_ctx)) {
            HandleImpl(recv_ctx);
            continue;
        }
        
        if (recv_queue->IsTerminated()) {
            running_ = false;
            size_t left = recv_queue->size();
            if (left > 0) {
                LogI("WorkerThread %zu tasks left", left)
            }
            break;
        }
    }
    
    LogI("Worker%d terminate!", thread_seq_)
}

void Server::WorkerThread::BindNetThread(Server::NetThread *_net_thread) {
    if (_net_thread) {
        net_thread_ = _net_thread;
    }
}

void Server::WorkerThread::NotifyStop() {
    if (!net_thread_) {
        LogE("!net_thread_, notify failed")
        return;
    }
    LogI("notify worker%d stop", thread_seq_)
    
    net_thread_->GetRecvQueue()->Terminate();
}

int Server::WorkerThread::__MakeWorkerThreadSeq() {
    static int thread_seq = 0;
    return ++thread_seq;
}

int Server::WorkerThread::GetWorkerSeqNum() const { return thread_seq_; }

Server::WorkerThread::~WorkerThread() = default;


Server::ConnectionManager::ConnectionManager()
        : socket_epoll_(nullptr) {
}

void Server::ConnectionManager::SetEpoll(SocketEpoll *_epoll) {
    if (_epoll) {
        socket_epoll_ = _epoll;
    }
}

tcp::ConnectionProfile *Server::ConnectionManager::GetConnection(SOCKET _fd) {
    ScopedLock lock(mutex_);
    auto find = connections_.find(_fd);
    if (find == connections_.end()) {
        return nullptr;
    }
    return find->second;
}

void Server::ConnectionManager::AddConnection(SOCKET _fd, std::string &_ip, uint16_t _port) {
    if (_fd < 0) {
        LogE("%d", _fd)
        LogPrintStacktrace()
        return;
    }
    ScopedLock lock(mutex_);
    if (connections_.find(_fd) != connections_.end()) {
        LogE("already got %d", _fd)
        return;
    }
    auto neo = new tcp::ConnectionProfile(_fd, _ip, _port);
    connections_.emplace(_fd, neo);
    if (socket_epoll_) {
        // While one thread is blocked in a call to epoll_wait(), it is
        // possible for another thread to add a file descriptor to the
        // waited-upon epoll instance.  If the new file descriptor becomes
        // ready, it will cause the epoll_wait() call to unblock.
        socket_epoll_->AddSocketReadWrite(_fd, (uint64_t) neo);
    }
}

void Server::ConnectionManager::DelConnection(SOCKET _fd) {
    ScopedLock lock(mutex_);
    auto &conn = connections_[_fd];
    if (socket_epoll_) {
        socket_epoll_->DelSocket(_fd);
    }
    if (conn) {
        LogI("fd(%d)", _fd)
    } else {
        LogE("fd(%d) Bug here!!", _fd)
    }
    delete conn, conn = nullptr;
    connections_.erase(_fd);
    
}

void Server::ConnectionManager::ClearTimeout() {
    uint64_t now = ::gettickcount();
    ScopedLock lock(mutex_);
    auto it = connections_.begin();
    while (it != connections_.end()) {
        if (it->second->IsTimeout(now)) {
            LogI("clear fd: %d", it->first)
            socket_epoll_->DelSocket(it->first);
            delete it->second, it->second = nullptr;
            it = connections_.erase(it);
            continue;
        }
        ++it;
    }
}

Server::ConnectionManager::~ConnectionManager() {
    ScopedLock lock(mutex_);
    for (auto &it : connections_) {
        delete it.second, it.second = nullptr;
    }
}


Server::NetThread::NetThread()
        : Thread() {
    
    connection_manager_.SetEpoll(&socket_epoll_);
    epoll_notifier_.SetSocketEpoll(&socket_epoll_);
}

void Server::NetThread::Run() {
    LogI("launching NetThread!")
    int epoll_retry = 3;
    
    const uint64_t clear_timeout_period = 3000;
    uint64_t last_clear_ts = 0;
    
    while (true) {
        
        int n_events = socket_epoll_.EpollWait(clear_timeout_period);
        
        if (n_events < 0) {
            if (socket_epoll_.GetErrNo() == EINTR || --epoll_retry > 0) {
                continue;
            }
            break;
        }
    
        for (int i = 0; i < n_events; ++i) {
            EpollNotifier::Notification probable_notification(
                    socket_epoll_.GetEpollDataPtr(i));
            
            if (__IsNotifySend(probable_notification)) {
                HandleSend();
                continue;
                
            } else if (__IsNotifyStop(probable_notification)) {
                LogI("NetThread notification-stop, break")
                running_ = false;
                __NotifyWorkersStop();
                return;
            }
            
            tcp::ConnectionProfile *profile;
            if ((profile = (tcp::ConnectionProfile *) socket_epoll_.IsReadSet(i))) {
                SOCKET fd = profile->FD();
                __OnReadEvent(fd);
            }
            if ((profile = (tcp::ConnectionProfile *) socket_epoll_.IsWriteSet(i))) {
                auto send_ctx = profile->GetSendContext();
                __OnWriteEvent(send_ctx);
            }
            if ((profile = (tcp::ConnectionProfile *) socket_epoll_.IsErrSet(i))) {
                SOCKET fd = profile->FD();
                __OnErrEvent(fd);
            }
        }
    
        uint64_t now = ::gettickcount();
        if (now - last_clear_ts > clear_timeout_period) {
            last_clear_ts = now;
            ClearTimeout();
        }
    }
    
}

void Server::NetThread::NotifySend() {
    epoll_notifier_.NotifyEpoll(notification_send_);
}

void Server::NetThread::NotifyStop() {
    running_ = false;
    epoll_notifier_.NotifyEpoll(notification_stop_);
}

void Server::NetThread::AddConnection(SOCKET _fd, std::string &_ip, uint16_t _port) {
    if (_fd < 0) {
        LogE("invalid fd: %d", _fd)
        return;
    }
    connection_manager_.AddConnection(_fd, _ip, _port);
}

void Server::NetThread::DelConnection(SOCKET _fd) {
    if (_fd < 0) {
        LogE("invalid fd: %d", _fd)
        return;
    }
    connection_manager_.DelConnection(_fd);
}

void Server::NetThread::HandleSend() {
    tcp::SendContext *send_ctx;
    while (send_queue_.pop_front_to(send_ctx, false)) {
        LogI("fd(%d) doing send task", send_ctx->fd)
        __OnWriteEvent(send_ctx);
    }
}

void Server::NetThread::ClearTimeout() { connection_manager_.ClearTimeout(); }

Server::RecvQueue *Server::NetThread::GetRecvQueue() { return &recv_queue_; }

Server::SendQueue *Server::NetThread::GetSendQueue() { return &send_queue_; }

void Server::NetThread::BindNewWorker(Server::WorkerThread *_worker) {
    if (_worker) {
        workers_.emplace_back(_worker);
        _worker->BindNetThread(this);
    }
}

void Server::NetThread::HandleException(std::exception &ex) {
    LogE("%s", ex.what())
}

bool Server::NetThread::__IsNotifySend(EpollNotifier::Notification &_notification) const {
    return _notification == notification_send_;
}

bool Server::NetThread::__IsNotifyStop(EpollNotifier::Notification &_notification) const {
    return _notification == notification_stop_;
}

int Server::NetThread::__OnReadEvent(SOCKET _fd) {
    if (_fd <= 0) {
        LogE("invalid _fd: %d", _fd)
        return -1;
    }
    LogI("fd: %d", _fd)
    
    tcp::ConnectionProfile *conn = connection_manager_.GetConnection(_fd);
    if (!conn) {
        LogE("conn == NULL, fd: %d", _fd)
        return -1;
    }
    
    if (conn->Receive() < 0) {
        DelConnection(_fd);
        return -1;
    }
    
    if (conn->IsParseDone()) {
        LogI("fd(%d) http parse succeed", _fd)
        recv_queue_.push_back(conn->GetRecvContext());
    }
    return 0;
}

int Server::NetThread::__OnWriteEvent(tcp::SendContext *_send_ctx) {
    if (!_send_ctx) {
        LogE("!_send_ctx")
        return -1;
    }
    AutoBuffer &resp = _send_ctx->buffer;
    size_t pos = resp.Pos();
    size_t ntotal = resp.Length() - pos;
    SOCKET fd = _send_ctx->fd;
    
    if (ntotal == 0 || fd < 0) {
        LogI("fd(%d), nothing to send", fd)
        return 0;
    }
    
    ssize_t nsend = ::write(fd, resp.Ptr(pos), ntotal);
    
    do {
        if (nsend == ntotal) {
            LogI("send %zd/%zu B, done", nsend, ntotal)
            break;
        }
        if (nsend >= 0 || (nsend < 0 && IS_EAGAIN(errno))) {
            nsend = nsend > 0 ? nsend : 0;
            LogI("fd(%d): send %zd/%zu B", fd, nsend, ntotal)
            resp.Seek(pos + nsend);
            return 0;
        }
        if (nsend < 0) {
            if (errno == EPIPE) {
                // fd probably closed by peer, or cleared because of timeout.
                LogI("fd(%d) already closed, send nothing", fd)
                return 0;
            }
            LogE("fd(%d) nsend(%zd), errno(%d): %s",
                 fd, nsend, errno, strerror(errno));
        }
    } while (false);
    
    DelConnection(fd);
    
    return nsend < 0 ? -1 : 0;
}

int Server::NetThread::__OnErrEvent(int _fd) {
    LogE("fd: %d", _fd)
    DelConnection(_fd);
    return 0;
}

int Server::NetThread::__OnReadEventTest(SOCKET _fd) {
    LogI("sleeping...")
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    char buff[1024] = {0, };
    
    while (true) {
        ssize_t n = ::read(_fd, buff, 2);
        if (n == -1 && IS_EAGAIN(errno)) {
            LogI("EAGAIN")
            return 0;
        }
        if (n == 0) {
            LogI("Conn closed by peer")
            break;
        }
        if (n < 0) {
            LogE("err: n=%zd", n)
            break;
        }
        LogI("n: %zd", n)
        if (n > 0) {
            LogI("read: %s", buff)
        }
    }
    DelConnection(_fd);
    return 0;
}

void Server::NetThread::OnStarted() {
    if (workers_.empty()) {
        LogE("call Server::SetWorker() to employ workers first!")
        assert(false);
    }
    // try launching all workers.
    for (auto & worker_thread : workers_) {
        worker_thread->Start();
    }
}

void Server::NetThread::OnWorkersHighPressure() {
    // TODO: Redesign strategy.
    const size_t kThreshHold = 100;
    if (recv_queue_.size() > kThreshHold) {
        // hire more workers
    } else if (workers_.size() > 1) {
        // stop unnecessary workers
    }
}

void Server::NetThread::__NotifyWorkersStop() {
    recv_queue_.Terminate();
    for (auto & worker_thread : workers_) {
        int seq = worker_thread->GetWorkerSeqNum();
        worker_thread->Join();
        delete worker_thread, worker_thread = nullptr;
        LogI("WorkerThread%d dead", seq)
    }
}

Server::NetThread::~NetThread() = default;

void Server::__NotifyNetThreadsStop() {
    for (auto & net_thread : net_threads_) {
        net_thread->NotifyStop();
        net_thread->Join();
        delete net_thread, net_thread = nullptr;
        LogI("NetThread dead")
    }
    LogI("All Threads Joined!")
}

bool Server::__IsNotifyStop(EpollNotifier::Notification &_notification) const {
    return _notification == notification_stop_;
}

int Server::__OnConnect() {
    SOCKET fd;
    struct sockaddr_in sock_in{};
    socklen_t socklen = sizeof(sockaddr_in);
    char ip_str[INET_ADDRSTRLEN] = {0, };
    uint16_t port = 0;
    
    while (true) {
        fd = ::accept(listenfd_, (struct sockaddr *) &sock_in, &socklen);
        if (fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            if (IS_EAGAIN(errno)) {
                return 0;
            }
            LogE("errno(%d): %s", errno, strerror(errno));
            return -1;
        }
        if (!inet_ntop(AF_INET, &sock_in.sin_addr, ip_str, sizeof(ip_str))) {
            LogE("inet_ntop errno(%d): %s", errno, strerror(errno))
        }
        port = ntohs(sock_in.sin_port);
        std::string ip(ip_str);
        LogI("fd(%d), address: [%s:%d]", fd, ip_str, port);
        SetNonblocking(fd);
        __AddConnection(fd, ip, port);
    }
}

void Server::__AddConnection(SOCKET _fd, std::string &_ip, uint16_t _port) {
    if (_fd < 0) {
        return;
    }
    uint net_thread_idx = _fd % net_thread_cnt_;
    NetThread *owner_thread = net_threads_[net_thread_idx];
    if (!owner_thread) {
        LogE("wtf???")
        return;
    }
    owner_thread->AddConnection(_fd, _ip, _port);
    
}

int Server::__CreateListenFd() {
    listenfd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd_ < 0) {
        LogE("create socket error: %s, errno: %d",
             strerror(errno), errno);
        return -1;
    }
    // FIXME
    struct linger ling{};
    ling.l_linger = 0;
    ling.l_onoff = 1;
    ::setsockopt(listenfd_, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
    SetNonblocking(listenfd_);
    return 0;
}


int Server::__Bind(uint16_t _port) const {
    struct sockaddr_in sock_addr{};
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(_port);
    
    int ret = ::bind(listenfd_, (struct sockaddr *) &sock_addr,
                        sizeof(sock_addr));
    if (ret < 0) {
        LogE("errno(%d): %s", errno, strerror(errno));
        return -1;
    }
    return 0;
}

int Server::__OnEpollErr(int _fd) {
    LogE("fd: %d", _fd)
    return 0;
}

