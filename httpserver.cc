#include "httpserver.h"
#include "utils/log.h"
#include "netscene/netscenedispatcher.h"
#include "socket/socketepoll.h"
#include "socket/blocksocket.h"
#include "http/httprequest.h"
#include "http/parsermanager.h"
#include "utils/threadpool.h"
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <fcntl.h>


const int HttpServer::kBuffSize = 1024;


HttpServer::HttpServer()
    : listenfd_(-1)
    , running_(true) {}


void HttpServer::Run(uint16_t _port) {
    if (__CreateListenFd() < 0) { return; }
    if (__Bind(_port) < 0) { Stop(); }
    
    listen(listenfd_, 1024); // 1024 defines the maximum length for the queue of pending connections
    
    SocketEpoll::Instance().SetListenFd(listenfd_);
    
    while (running_) {
        int nfds = SocketEpoll::Instance().EpollWait();
        LogI("[HttpServer::Run] nfds: %d", nfds)
        
        for (int i = 0; i < nfds; i++) {
            int fd;
            if (SocketEpoll::Instance().IsNewConnect(i)) {
                LogI("IsNewConnect")
                __HandleConnect();
                
            } else if ((fd = SocketEpoll::Instance().IsReadSet(i)) > 0) {
                LogI("IsReadSet, fd: %d", fd)
//                ThreadPool::Instance().Execute([=] {
                    __HandleRead(fd);
//                    return __HandleRead(fd);
//                });
                
            } else if ((fd = SocketEpoll::Instance().IsErrSet(i)) > 0) {
                LogE("[HttpServer::Run] IsErrSet, fd:%d, i:%d", fd, i)
            }
        }
    }
    Stop();
}

int HttpServer::__HandleRead(SOCKET _fd) {
    using http::request::ParserManager;
    auto parser = ParserManager::Instance().GetParser(_fd);
    
    AutoBuffer *recv_buff = parser->GetBuff();
    while (true) {
        size_t available = recv_buff->AvailableSize();
        if (available < kBuffSize) {
            recv_buff->AddCapacity(kBuffSize - available);
        }
        ssize_t n = recv(_fd, recv_buff->Ptr(recv_buff->Length()),
                         kBuffSize, 0);
        if (n == -1 && errno == EAGAIN) {
            // no messages available and fd is nonblocking,
            LogI("[HttpServer::__HandleRead] EAGAIN")
            return 0;
        }
        if (n == 0) {
            LogI("[HttpServer::__HandleRead] Conn closed by peer")
            break;
        }
        if (n > 0) { recv_buff->AddLength(n); }

        LogI("[HttpServer::__HandleRead] n: %zd", n)
        parser->DoParse();
        
        if (parser->IsEnd() || parser->IsErr()) {
            ParserManager::Instance().DeleteParser(_fd);
            SocketEpoll::Instance().DelSocket(_fd);
            if (parser->IsErr()) {
                LogE("[HttpServer::__HandleRead] parser error")
                return -1;
            }
            
            break;
        }
    }
    int ret = NetSceneDispatcher::Instance().Dispatch(_fd, parser->GetBody());
    close(_fd);
    return ret;
}

int HttpServer::__HandleConnect() {
    int fd = accept(listenfd_, (struct sockaddr *) NULL, NULL);
    if (fd < 0) {
        LogE("[HttpServer::__HandleConnect] accept socket error: %s, errno: %d",
             strerror(errno), errno);
        return fd;
    }
    int old_flags = fcntl(fd, F_GETFL);
    if (fcntl(fd, F_SETFL, old_flags | O_NONBLOCK) == -1) {
        LogE("[HttpServer::__HandleConnect] fcntl return -1")
        return -1;
    }
    LogI("[HttpServer::__HandleConnect] new connect, fd: %d", fd);
    return SocketEpoll::Instance().AddSocketRead(fd);
}

int HttpServer::__CreateListenFd() {
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd_ < 0) {
        LogE("[HttpServer::__CreateListenFd] create socket"
             " error: %s, errno: %d", strerror(errno), errno);
        return -1;
    }
    // TODO
    struct linger ling;
    ling.l_linger = 0;
    ling.l_onoff = 1;
    setsockopt(listenfd_, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
    return 0;
}

int HttpServer::__Bind(uint16_t _port) {
    struct sockaddr_in sock_addr;
    
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(_port);
    
    int ret = bind(listenfd_, (struct sockaddr *) &sock_addr,
                        sizeof(sock_addr)); // create a special socket file
    if (ret < 0) {
        LogE("[HttpServer::__Bind] errno(%d): %s", errno, strerror(errno));
        return -1;
    }
    return 0;
}

void HttpServer::Stop() {
    running_ = false;
    if (listenfd_ != -1) {
        close(listenfd_);
    }
    exit(0);
}
