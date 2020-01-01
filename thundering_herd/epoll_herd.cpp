//
// Created by gjk on 2019/12/30.
// 模拟多进程的epoll惊群

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include <sys/epoll.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include <fcntl.h>
#include <errno.h>
const int PROCESS_NUM = 10;
const int MAXEVENTS = 64;

int get_listened_sock(int port){
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(::bind(fd, (struct sockaddr*)&addr, sizeof addr)<0){
        fprintf(stderr, "bind error, err=%s\n", strerror(errno));
        return -1;
    }

    int val = ::fcntl(fd, F_GETFL);
    val |= O_NONBLOCK;
    if(::fcntl(fd, F_SETFL, val) < 0){
        fprintf(stderr, "set nonblock error\n");
        return -1;
    }

    if(::listen(fd, 20)<0){
        fprintf(stderr, "listen error\n");
        return -1;
    }
    return fd;
}

int main(){

    int sock = get_listened_sock(9985);
    int epollfd = ::epoll_create(MAXEVENTS);
    if(epollfd<0){
        fprintf(stderr, "epoll error, err=%s\n", strerror(errno));
        return -1;
    }
    struct epoll_event event;
    event.data.fd = sock;
    event.events = EPOLLIN;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &event) < 0){
        fprintf(stderr, "epoll add error err=%s\n", strerror(errno));
        return -1;
    }

    struct epoll_event* events = (struct epoll_event*)calloc(MAXEVENTS, sizeof(epoll_event));
    for(int i = 0;i<PROCESS_NUM;++i){
        pid_t pid = fork();
        if(pid==0){
            while(1){
                int num = ::epoll_wait(epollfd, events, MAXEVENTS, -1);
                ::sleep(2);
                fprintf(stderr, "process pid=%d returned num=%d from epoll_wait\n", getpid(), num);
                for(int j = 0;j<num; ++j){
                    if(events[i].events&EPOLLERR || events[i].events&EPOLLHUP){
                        fprintf(stderr, "epoll error, e=%d,fd=%d\n", events[i].events, events[i].data.fd);
                        ::close(events[i].data.fd);
                        continue;
                    }else if(events[i].events&EPOLLIN) {
                        if (events[i].data.fd == sock) {
                            int csock;
                            if ((csock=::accept(sock, NULL, NULL)) < 0) {
                                fprintf(stderr, "process pid=%d e=%d accept failed\n", getpid(), events[i].events);
                            } else {
                                fprintf(stderr, "process pid=%d e=%d accept success\n", getpid(), events[i].events);
                                ::close(csock);
                            }
                        }else fprintf(stderr, "event fd=%d\n", events[i].data.fd);
                    }else {
                        fprintf(stderr, "Unknown event=%d\n", events[i].events);
                    }
                }
            }
        }
    }

    wait(NULL);
    free(events);
    ::close(sock);
    return 0;
}