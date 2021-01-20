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
#include "log.h"


const int kMaxLine = 1024;
int socket_;

bool Connect() {
    char svrInetAddr[] = "127.0.0.1";
    
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ <= 0) return false;
    
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(5002);
    sockaddr.sin_addr.s_addr = inet_addr(svrInetAddr);   // inet_pton(AF_INET, svrInetAddr, &sockaddr.sin_addr);
    
    if (connect(socket_, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
        LogI("connect failed");
        close(socket_);
        return false;
    }
    LogI("connect succeed!");
    return true;
}


void *StartRoutine(void *arg) {
    int i = *(int *) arg;
    printf("Thread working... %d \n", i);
    if (!Connect()) {
        return NULL;
    }
    
    char sendline[100];
    while (strcmp(sendline, "q") != 0) {
        printf("--hi--: \n");
        fgets(sendline, 1024, stdin);
        if (send(socket_, sendline, strlen(sendline), 0) < 0) {
            printf("send msg error: %s errno: %d\n", strerror(errno), errno);
            break;
        }
    }
    close(socket_);
    printf("exit\n");
    
    return NULL;
}

//int main(int argc, char **argv) {
//    pthread_t tid;
//    int arg = 2;
//    pthread_create(&tid, NULL, StartRoutine, &arg);
//    printf("%d\n", pthread_join(tid, NULL));
////    sleep(1);
//    return 0;
//}

