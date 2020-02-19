//
// Created by gjk on 2019/12/30.
// 模拟accept惊群，但是accept惊群是已经被linux内核处理了的，
// 所以这里实际是不会发生惊群现象的
//

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>

const int PROCESS_NUM = 10;
int get_listened_sock(int port){
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(::bind(fd, (struct sockaddr*)&addr, sizeof addr)<0){
        fprintf(stderr, "bind error\n");
        return -1;
    }

    if(::listen(fd, 20)<0){
        fprintf(stderr, "listen error\n");
        return -1;
    }
    return fd;
}
int main(){
    int ssock = get_listened_sock(9981);
    for(int i = 0;i<PROCESS_NUM;++i){
        pid_t pid = ::fork();
        if(pid==0){
            while(1){
                int csock = ::accept(ssock, NULL, NULL);
                fprintf(stderr, "process pid=%d， accept returned, csock=%d。\n", getpid(), csock);
                char buf[] = "Ni,hao!\n";
                int ret = ::send(csock, buf, sizeof buf, 0);
                fprintf(stderr, "process pid=%d send num=%d\n", getpid(), ret);
                ::close(csock);
            }
        }
    }
    ::wait(NULL);
    return 0;
}