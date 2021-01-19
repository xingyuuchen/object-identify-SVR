#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include "log.h"
#include "netscenedispatcher.h"


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
    Log("signal: %d, process Exit", _sig);
    running = 0;
    if (_sig == 2) {
        Exit();
    }
}


int main(int argc, char **argv) {
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
        Log("create socket error: %s errno :%d", strerror(errno), errno);
        return -1;
    }
    
    bind_res = bind(listenfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)); // create a special socket file
    listen(listenfd, 1024); // 1024 defines the maximum length for the queue of pending connections
    
    while (running) {
        if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1) {
            Log("accept socket error: %s errno :%d", strerror(errno), errno);
            continue;
        }
        Log("new connect");
        
        AutoBuffer recv_buff;
    
        int security = 0;
        while (security < 1024) {
            size_t nsize = UnixSocket::BlockSocketReceive(connfd, recv_buff, kBuffSize);
            if (nsize <= 0) {
                Log("UnixSocket::BlockSocketReceive ret: %zd", nsize);
                break;
            }
            if (nsize < kBuffSize) {
                Log("UnixSocket::BlockSocketReceive ret: %zd", nsize);
                break;
            }
            security++;
        }
        
        NetSceneDispatcher::GetInstance().Dispatch(connfd, &recv_buff);
        
        close(connfd);
    }
    Exit();
}


