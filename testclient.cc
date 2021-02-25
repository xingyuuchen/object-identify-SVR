#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "utils/log.h"


const int kBuffSize = 1024;
int socket_;

bool Connect() {
    char svrInetAddr[] = "127.0.0.1";
    
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ <= 0) return false;
    
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(5002);
    sockaddr.sin_addr.s_addr = inet_addr(svrInetAddr);
    
    if (::connect(socket_, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
        LogI("connect failed");
        ::close(socket_);
        return false;
    }
    LogI("connect succeed!");
    return true;
}


int main() {
    if (!Connect()) {
        return 0;
    }
    
    char sendline[100];
    while (strcmp(sendline, "q") != 0) {
        printf("--hi--: \n");
        fgets(sendline, 1024, stdin);
        if (::send(socket_, sendline, strlen(sendline), 0) < 0) {
            printf("send errno(%d): %s\n", errno, strerror(errno));
            break;
        }
    }
    ::close(socket_);
    
    return 0;
}

