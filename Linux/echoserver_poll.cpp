//
// Created by gjk on 2020/2/29.
// 基于poll的IO多路复用的echoserver
// poll监听serverfd和stdin
//

#include "util.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/poll.h>
#include <string.h>
const int PORT = 9876;

void echostdin(){
    char buf[1024];
    if(!fgets(buf, 1024, stdin)){
        //如果对标准输入键入EOF，那么select会不断返回，因为它认为标准输出已经到EOF了，而读EOF会返回0，就这样死循环下去了
        exit(0);
    }
    printf("%s", buf);
}

int main(){
    int serverfd = open_listenedfd(PORT);

    struct pollfd fds[2];
    memset(fds, 0, sizeof(fds));
    //添加标准输入的fd
    fds[0].fd = STDIN_FILENO;   //poll监听的描述符
    fds[0].events = POLLIN; //我们关心的事件
    //添加serverfd
    fds[1].fd = serverfd;
    fds[1].events = POLLIN;

    while(true){
        //这里第一个参数应该是fd_set中最大的fd+1
        int ret = ::poll(fds, 2, -1);
        if(ret<0 && errno != EINTR){
            perror("poll happened error");
            break;
        }
        //检查标准输入
        if(fds[0].revents & POLLIN){
            //从标准输入读，然后写到标准输出
            echostdin();
        }
        //检查serverfd
        if(fds[1].revents & POLLIN){
            struct sockaddr clientaddr;
            socklen_t socklen = sizeof(clientaddr);
            int clientfd = ::accept(serverfd, &clientaddr, &socklen);
            char ipport[100];
            get_ipport_str(&clientaddr, socklen, ipport, sizeof(ipport));
            printf("recv connect from client: %s\n", ipport);
            //为了简单，没有把clientfd加入到poll中进行监听，而是直接echo处理完
            echo(clientfd, clientfd);
            ::close(clientfd);
            printf("close connect from client: %s\n", ipport);
        }
    }

    return 0;
}