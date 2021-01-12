#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include "netscenedispatcher.h"


const int kMaxLine = 128;

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
    printf("Stop is captured, %d\n", _sig);
    running = 0;
    if (_sig == 2) {
        Exit();
    }
}


int main() {
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
        printf("create socket error: %s errno :%d\n", strerror(errno), errno);
        return -1;
    }

    bind_res = bind(listenfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)); // create a special socket file
    listen(listenfd, 1024); // 1024 defines the maximum length for the queue of pending connections

    while (running) {
        if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1) {
            printf("accept socket error: %s errno :%d\n", strerror(errno), errno);
            continue;
        }
        AutoBuffer recv_buff;
        recv_buff.AddCapacity(128);
        ssize_t n = recv(connfd, recv_buff.Ptr(), kMaxLine, 0);
        recv_buff.SetLength(n);

        NetSceneDispatcher::GetInstance().Dispatch(connfd, &recv_buff);

        close(connfd);
    }
    Exit();
}


