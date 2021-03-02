#include "httpserver.h"
#include "utils/log.h"
#include "netscene/netscenedispatcher.h"
#include "socket/socketepoll.h"
#include "socket/blocksocket.h"
#include "http/httprequest.h"
#include "http/parsermanager.h"
#include "utils/threadpool/threadpool.h"
#include "signalhandler.h"
#include <unistd.h>
#include <iostream>
#include <string.h>


const int HttpServer::kBuffSize = 1024;

const char *const HttpServer::TAG = "HttpServer";

HttpServer::HttpServer()
        : listenfd_(-1)
        , running_(true) {
    
    SignalHandler::Instance().Init();
}


void HttpServer::Run(uint16_t _port) {
    if (__CreateListenFd() < 0) { return; }
    if (__Bind(_port) < 0) { return; }
    
    // 1024 defines the maximum length for the queue of pending connections
    ::listen(listenfd_, 1024);
    
    SocketEpoll::Instance().SetListenFd(listenfd_);
    
    while (running_) {
        int nfds = SocketEpoll::Instance().EpollWait();
        LogI(TAG, "[Run] nfds: %d", nfds)
        if (nfds < 0) { break; }
        
        for (int i = 0; i < nfds; i++) {
            if (SocketEpoll::Instance().IsNewConnect(i)) {
                __HandleConnect();
                
            } else if (SOCKET fd = SocketEpoll::Instance().IsReadSet(i)) {
//                __HandleReadTest(fd);
                ThreadPool::Instance().Execute(fd, [=] { return __HandleRead(fd); });
    
            } else if (void *ptr = SocketEpoll::Instance().IsWriteSet(i)) {
                NetSceneBase *net_scene = (NetSceneBase *) ptr;
                ThreadPool::Instance().Execute(net_scene->GetSocket(), [=] {
                    return __HandleWrite(net_scene, false);
                });
    
            } else if (SOCKET fd = SocketEpoll::Instance().IsErrSet(i)) {
                ThreadPool::Instance().Execute(fd, [=] { return __HandleErr(fd); });
            }
        }
    }
}

int HttpServer::__HandleRead(SOCKET _fd) {
    if (_fd <= 0) {
        LogE(TAG, "[__HandleRead] invalid _fd: %d", _fd)
        return -1;
    }
    LogI(TAG, "[__HandleRead] _fd: %d", _fd)
    
    using http::request::ParserManager;
    auto parser = ParserManager::Instance().GetParser(_fd);

    AutoBuffer *recv_buff = parser->GetBuff();

    while (true) {
        size_t available = recv_buff->AvailableSize();
        if (available < kBuffSize) {
            recv_buff->AddCapacity(kBuffSize - available);
        }

        ssize_t n = ::read(_fd, recv_buff->Ptr(
                recv_buff->Length()), kBuffSize);

        if (n == -1 && errno == EAGAIN) {
            LogI(TAG, "[__HandleRead] EAGAIN")
            return 0;
        }
        if (n < 0) {
            LogE(TAG, "[__HandleRead] err: n=%zd", n)
            break;

        } else if (n == 0) {
            // A read event is raised when conn closed by peer
            LogI(TAG, "[__HandleRead] Conn closed by peer")
            break;

        } else if (n > 0) {
            recv_buff->AddLength(n);
        }

        LogI(TAG, "[__HandleRead] n: %zd", n)
        
        parser->DoParse();
        
        if (parser->IsErr()) {
            LogE(TAG, "[__HandleRead] parser error")
            break;
        }
        
        if (parser->IsEnd()) {
            LogI(TAG, "[__HandleRead] http parse succeed")
            NetSceneBase* net_scene =
                    NetSceneDispatcher::Instance().Dispatch(_fd, parser->GetBody());
            if (net_scene) {
                LogI(TAG, "[__HandleRead] net_scene process done")
                ParserManager::Instance().DelParser(_fd);
                return __HandleWrite(net_scene, false);
            }
            break;
        }
        
        if (n < kBuffSize) return 0;
    }
    ParserManager::Instance().DelParser(_fd);
    SocketEpoll::Instance().DelSocket(_fd);
    ::close(_fd);
    return -1;
}

int HttpServer::__HandleReadTest(SOCKET _fd) {
    LogI(TAG, "[__HandleReadTest] sleeping...")
    sleep(4);
    char buff[kBuffSize] = {0, };
    
    while (true) {
        ssize_t n = ::read(_fd, buff, 2);
        if (n == -1 && errno == EAGAIN) {
            LogI(TAG, "[__HandleReadTest] EAGAIN")
            return 0;
        }
        if (n == 0) {
            LogI(TAG, "[__HandleReadTest] Conn closed by peer")
            break;
        }
        if (n < 0) {
            LogE(TAG, "[__HandleReadTest] err: n=%zd", n)
            break;
        }
        LogI(TAG, "[__HandleReadTest] n: %zd", n)
        if (n > 0) {
            LogI(TAG, "read: %s", buff)
        }
    }
    SocketEpoll::Instance().DelSocket(_fd);
    ::close(_fd);
    return 0;
}

int HttpServer::__HandleWrite(NetSceneBase *_net_scene, bool _mod_write) {
    if (_net_scene == NULL) {
        LogE(TAG, "[__HandleWrite] _net_scene = NULL")
        return -1;
    }
    AutoBuffer *resp = _net_scene->GetHttpResp();
    size_t pos = resp->Pos();
    size_t ntotal = resp->Length() - pos;
    SOCKET fd = _net_scene->GetSocket();
    
    ssize_t nsend = ::write(fd, resp->Ptr(pos), ntotal);
    
    do {
        if (nsend == ntotal) {
            LogI(TAG, "[__HandleWrite] send %zd/%zu bytes without epoll", nsend, ntotal)
            break;
        }
        if (nsend >= 0 || (nsend < 0 && errno == EAGAIN)) {
            nsend = nsend > 0 ? nsend : 0;
            LogI(TAG, "[__HandleWrite] fd(%d): send %zd/%zu bytes", fd, nsend, ntotal)
            _net_scene->GetHttpResp()->Seek(pos + nsend);
            if (_mod_write) {
                SocketEpoll::Instance().ModSocketWrite(fd, (void *)_net_scene);
            }
            return 0;
        }
        if (nsend < 0) {
            LogE(TAG, "[__TryWrite] nsend(%zu), errno(%d): %s",
                 nsend, errno, strerror(errno));
        }
    } while (false);
    
    SocketEpoll::Instance().DelSocket(fd);
    delete _net_scene;
    ::close(fd);
    return nsend < 0 ? -1 : 0;
}

int HttpServer::__HandleConnect() {
    LogI(TAG, "[__HandleConnect] IsNewConnect")
    SOCKET fd;
    while (true) {
        fd = ::accept(listenfd_, (struct sockaddr *) NULL, NULL);
        if (fd < 0) {
            if (errno == EAGAIN) { return 0; }
            LogE(TAG, "[__HandleConnect] errno(%d): %s",
                 errno, strerror(errno));
            return -1;
        }
        SetNonblocking(fd);
        LogI(TAG, "[__HandleConnect] new connect, fd: %d", fd);
        SocketEpoll::Instance().AddSocketRead(fd);
    }
}

int HttpServer::__HandleErr(int _fd) {
    LogE(TAG, "[__HandleErr] fd: %d", _fd)
    return 0;
}

int HttpServer::__CreateListenFd() {
    listenfd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd_ < 0) {
        LogE(TAG, "[__CreateListenFd] create socket"
             " error: %s, errno: %d", strerror(errno), errno);
        return -1;
    }
    // FIXME
    struct linger ling;
    ling.l_linger = 0;
    ling.l_onoff = 1;
    ::setsockopt(listenfd_, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
    SetNonblocking(listenfd_);
    return 0;
}

int HttpServer::__Bind(uint16_t _port) {
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(_port);
    
    int ret = ::bind(listenfd_, (struct sockaddr *) &sock_addr,
                        sizeof(sock_addr));
    if (ret < 0) {
        LogE(TAG, "[__Bind] errno(%d): %s", errno, strerror(errno));
        return -1;
    }
    return 0;
}

HttpServer::~HttpServer() {
    running_ = false;
    if (listenfd_ != -1) {
        LogI(TAG, "[~HttpServer] close listenfd")
        ::close(listenfd_);
    }
}
