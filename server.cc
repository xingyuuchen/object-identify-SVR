#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include "log.h"
#include "netscenedispatcher.h"
#include "socket/socketpoll.h"
#include "socket/blocksocket.h"
#include "http/httprequest.h"


const int kBuffSize = 1024;

int running = 1;
int listenfd = -1;
int bind_res = -1;

void Exit() {
    if (listenfd != -1) {
        close(listenfd);
    }
    exit(0);
}


void Stop(int _sig) {
    LogI("signal: %d, process Exit", _sig);
    running = 0;
    if (_sig == 2) {
        Exit();
    }
}


int main1(int argc, char **argv) {
    printf("Server On...\n");
    signal(2, Stop);

    struct sockaddr_in sock_addr;

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(5002);

    int connfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        LogI("create socket error: %s errno :%d", strerror(errno), errno);
        return -1;
    }

    bind_res = bind(listenfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)); // create a special socket file
    listen(listenfd, 1024); // 1024 defines the maximum length for the queue of pending connections
    
    SocketPoll socket_poll;
    while (running) {
        if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1) {
            LogI("accept socket error: %s errno :%d", strerror(errno), errno);
            continue;
        }
        LogI("new connect");
    
        socket_poll.SetEventRead(connfd);
        socket_poll.SetEventError(connfd);
        
        http::request::Parser parser;
        
        AutoBuffer recv_buff;
        while (true) {
            size_t nsize = BlockSocketReceive(connfd, recv_buff, socket_poll, kBuffSize);
            if (nsize <= 0) {
                LogI("BlockSocketReceive ret: %zd", nsize);
                break;
            }
            parser.Recv(recv_buff);
            if (parser.IsEnd()) {
                break;
            } else if (parser.IsErr()) {
                LogI("parser error")
                break;
            }
        }

        NetSceneDispatcher::GetInstance().Dispatch(connfd, &parser.GetBody());

        socket_poll.RemoveSocket(connfd);
        close(connfd);
    }
    Exit();
}

int main() {
//    LogE("ds")
}

