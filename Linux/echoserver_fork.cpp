//
// Created by gjk on 2020/2/29.
// 基于进程并发的echoserver
//

#include "util.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
const int PORT = 9876;

void child_die(int sig){
    if(sig == SIGCHLD){
        //-1：指定任何子进程
        //NULL：不需要返回值
        //WNOHANG：不阻塞立即返回，回收成功返回子进程ID，没有子进程退出返回0，出错返回-1
        pid_t pid;
        while((pid = ::waitpid(-1, NULL, WNOHANG)) > 0)
            printf("sub process %u exit\n", pid);
    }
}

int main(){

    //子进程退出的时候父进程会收到SIGCHLD信号，默认是忽略
    // 我们捕捉到它，然后在这里回收子进程资源
    ::signal(SIGCHLD, child_die);

    int serverfd = open_listenedfd(PORT);
    while(true){
        struct sockaddr clientaddr;
        socklen_t socklen = sizeof(clientaddr);
        int clientfd = ::accept(serverfd, &clientaddr, &socklen);
        char ipport[100];
        get_ipport_str(&clientaddr, socklen, ipport, sizeof(ipport));
        printf("recv connect from client: %s\n", ipport);

        //创建新进程来处理读写事件
        pid_t pid = fork();
        if(pid==0){
            //子进程会增加serverfd的引用计数，所以在子进程中要关掉它
            ::close(serverfd);
            echo(clientfd, clientfd);
            ::close(clientfd);
            printf("close connect from client: %s\n", ipport);
            exit(0);
        }
    }

    return 0;
}