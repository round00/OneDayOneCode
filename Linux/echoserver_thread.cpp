//
// Created by gjk on 2020/3/1.
// 基于线程并发的echoserver
//

#include "util.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
const int PORT = 9876;

void* thread_func(void* fd){
    //自己要主动分离，就不用主线程来释放资源了
    ::pthread_detach(::pthread_self());
    int clientfd = *(int*)fd;
    free(fd);
    echo(clientfd, clientfd);
    ::close(clientfd);
    return NULL;
}

int main(){
    int serverfd = open_listenedfd(PORT, 0);
    if(serverfd<0){
        return 0;
    }
    while(true){
        struct sockaddr clientaddr;
        socklen_t socklen = sizeof(clientaddr);
        int clientfd = ::accept(serverfd, &clientaddr, &socklen);
        char ipport[100];
        get_ipport_str(&clientaddr, socklen, ipport, sizeof(ipport));
        printf("recv connect from client: %s\n", ipport);
        int *pfd = (int*)malloc(sizeof(int));
        *pfd = clientfd;
        //创建新线程去处理
        pthread_t pid;
        ::pthread_create(&pid, NULL, thread_func, pfd);
        if(pid<0)break;
    }

    return 0;
}