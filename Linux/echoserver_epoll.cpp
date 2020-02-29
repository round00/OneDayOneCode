//
// Created by gjk on 2020/2/29.
// 基于epoll的IO多路复用的echoserver
// epoll监听serverfd和stdin
//

#include "util.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/epoll.h>
#include <string.h>
const int PORT = 9876;
const int MAX_EPOLL_EVENTS = 32;

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
    int epollfd = ::epoll_create(MAX_EPOLL_EVENTS);   //创建epoll的fd，参数是epoll最大监听的数目
    struct epoll_event events[MAX_EPOLL_EVENTS];    //这个是用于epoll有触发事件的event的
    struct epoll_event event = {0, {0}};
    //添加标准输入的fd
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;
    ::epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    //添加serverfd
    event.events = EPOLLIN;
    event.data.fd = serverfd;
    ::epoll_ctl(epollfd, EPOLL_CTL_ADD, serverfd, &event);

    while(true){
        //这里第一个参数应该是fd_set中最大的fd+1
        int nEvents = ::epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, -1);
        if(nEvents<0 && errno != EINTR){
            perror("poll happened error");
            break;
        }
        for(int i = 0;i<nEvents; ++i){
            //检查标准输入
            if(events[i].data.fd ==  STDIN_FILENO){
                if(events[i].events & EPOLLIN){
                    echostdin();
                }
            }
            //检查serverfd
            if(events[i].data.fd ==  serverfd){
                if(events[i].events & EPOLLIN){
                    struct sockaddr clientaddr;
                    socklen_t socklen = sizeof(clientaddr);
                    int clientfd = ::accept(serverfd, &clientaddr, &socklen);
                    char ipport[100];
                    get_ipport_str(&clientaddr, socklen, ipport, sizeof(ipport));
                    printf("recv connect from client: %s\n", ipport);
                    //为了简单，没有把clientfd加入到epoll中进行监听，而是直接echo处理完
                    echo(clientfd, clientfd);
                    ::close(clientfd);
                    printf("close connect from client: %s\n", ipport);
                }
            }
        }
    }

    return 0;
}