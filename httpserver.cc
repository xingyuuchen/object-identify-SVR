#include "httpserver.h"
#include "utils/log.h"
#include "netscene/netscenedispatcher.h"
#include "socket/socketepoll.h"
#include "socket/blocksocket.h"
#include "http/httprequest.h"
#include "utils/threadpool.h"
#include <unistd.h>
#include <iostream>
#include <string.h>


const int HttpServer::kBuffSize = 1024;


HttpServer::HttpServer()
    : listenfd_(-1)
    , running_(true) {}


void HttpServer::Run(uint16_t _port) {
    if (__CreateListenFd() < 0) { return; }
    if (__Bind(_port) < 0) { Stop(); }
    
    listen(listenfd_, 1024); // 1024 defines the maximum length for the queue of pending connections
    
    SocketEpoll::Instance().SetListenFd(listenfd_);
    int nfds;
    while (running_) {
        nfds = SocketEpoll::Instance().EpollWait();
        LogI("[HttpServer::Run] nfds: %d", nfds)
        
        for (int i = 0; i < nfds; i++) {
            int fd;
            if (SocketEpoll::Instance().IsNewConnect(i)) {
                __HandleConnect();
                
            } else if ((fd = SocketEpoll::Instance().IsReadSet(i)) > 0) {
                LogI("IsReadSet, fd: %d", fd)
//                ThreadPool::Instance().Execute([=] {
                    http::request::Parser parser;
        
                    AutoBuffer recv_buff;
                    while (true) {
                        size_t nsize = BlockSocketReceiveWithoutPoll(fd, recv_buff, kBuffSize);
                        if (nsize < 0) {
                            LogE("BlockSocketReceive ret: %zd", nsize);
                            break;
                        }
                        
                        parser.Recv(recv_buff);
                        
                        if (parser.IsEnd()) {
                            break;
                        } else if (parser.IsErr()) {
                            LogE("parser error")
                            break;
                        }
                    }
                    NetSceneDispatcher::Instance().Dispatch(fd, &parser.GetBody());
        
                    close(fd);
//                    return 0;
//                });
                
            } else if ((fd = SocketEpoll::Instance().IsErrSet(i)) > 0) {
                LogE("[HttpServer::Run] IsErrSet, fd:%d, i:%d", fd, i)
            }
        }
    }
    Stop();
}

int HttpServer::__CreateListenFd() {
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd_ < 0) {
        LogE("[HttpServer::__CreateListenFd] create socket"
             " error: %s, errno: %d", strerror(errno), errno);
        return -1;
    }
    return 0;
}

int HttpServer::__Bind(uint16_t _port) {
    struct sockaddr_in sock_addr;
    
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(_port);
    
    int bind_res = bind(listenfd_, (struct sockaddr *) &sock_addr,
            sizeof(sock_addr)); // create a special socket file
    if (bind_res < 0) {
        LogE("[HttpServer::__Bind] bind error: %s, errno: %d", strerror(errno), errno);
        return -1;
    }
    return 0;
}

int HttpServer::__HandleConnect() {
    int fd = accept(listenfd_, (struct sockaddr *) NULL, NULL);
    if (fd > 0) {
        LogI("[HttpServer::__HandleConnect] new connect, fd: %d", fd);
        return SocketEpoll::Instance().AddSocketRead(fd);
    } else {
        LogE("[HttpServer::__HandleConnect] accept socket error: %s, errno: %d",
             strerror(errno), errno);
        return fd;
    }
}

void HttpServer::Stop() {
    running_ = false;
    if (listenfd_ != -1) {
        close(listenfd_);
    }
    exit(0);
}
