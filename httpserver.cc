#include "httpserver.h"
#include "utils/log.h"
#include "netscene/netscenedispatcher.h"
#include "socket/socketpoll.h"
#include "socket/blocksocket.h"
#include "http/httprequest.h"
#include <unistd.h>
#include <iostream>
#include <string.h>
#include "utils/threadpool.h"


HttpServer::HttpServer()
    : listenfd_(-1)
    , running_(true) {}

const int HttpServer::kBuffSize = 1024;


void HttpServer::Stop() {
    running_ = false;
    if (listenfd_ != -1) {
        close(listenfd_);
    }
    exit(0);
}

void HttpServer::Run(uint16_t _port) {
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd_ < 0) {
        LogE("create socket error: %s errno :%d", strerror(errno), errno);
        return;
    }
    
    struct sockaddr_in sock_addr;
    
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(_port);
    
    int bind_res = bind(listenfd_, (struct sockaddr *) &sock_addr, sizeof(sock_addr)); // create a special socket file
    if (bind_res < 0) {
        LogE("bind error: %s errno :%d", strerror(errno), errno);
        return;
    }
    int connfd;
    
    listen(listenfd_, 1024); // 1024 defines the maximum length for the queue of pending connections
    
    SocketPoll socket_poll;
    while (running_) {
        if ((connfd = accept(listenfd_, (struct sockaddr *) NULL, NULL)) == -1) {
            LogE("accept socket error: %s errno :%d", strerror(errno), errno);
            continue;
        }
        LogI("new connect");
        
        ThreadPool::Instance().Execute([=, &socket_poll] {
            socket_poll.SetEventRead(connfd);
            socket_poll.SetEventError(connfd);
            
            http::request::Parser parser;
    
            AutoBuffer recv_buff;
            while (true) {
                size_t nsize = BlockSocketReceive(connfd, recv_buff, socket_poll, kBuffSize);
                if (nsize <= 0) {
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
            NetSceneDispatcher::Instance().Dispatch(connfd, &parser.GetBody());

            socket_poll.RemoveSocket(connfd);
            close(connfd);
        });
        
    }
    Stop();
}
