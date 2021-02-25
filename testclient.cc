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


const int kBuffSize = 1024;
int socket_;

bool Connect() {
    char svrInetAddr[] = "49.235.29.121";
    
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ <= 0) return false;
    
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(5002);
    sockaddr.sin_addr.s_addr = inet_addr(svrInetAddr);
    
    if (::connect(socket_, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
        printf("connect failed\n");
        ::close(socket_);
        return false;
    }
    printf("connect succeed!\n");
    return true;
}


int main() {
    if (!Connect()) {
        return 0;
    }
    
    char sendline[9] = {'c', 'x', 'y', '\n', 'h', 'h', 'h', '\n', 0};
    
    for (int i = 0; i < 2; ++i) {
        sleep(2);
        printf("%zu: %s", strlen(sendline), sendline);
        if (::send(socket_, sendline, strlen(sendline), 0) < 0) {
            printf("send errno(%d): %s\n", errno, strerror(errno));
            break;
        }
    }
    sleep(10);
    
    ::close(socket_);
    
    return 0;
}

