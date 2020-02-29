//
// Created by gjk on 2020/2/29.
// 基于select的IO多路复用的echoserver
// select监听serverfd和stdin
//

#include "util.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/select.h>
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

    fd_set read_set, ready_set;
    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);    //添加标准输入的fd
    FD_SET(serverfd, &read_set);        //添加serverfd

    while(true){
        ready_set = read_set;
        //这里第一个参数应该是fd_set中最大的fd+1
        int ret = ::select(serverfd+1, &ready_set, NULL, NULL, NULL);
        if(ret<0 && errno != EINTR){
            perror("select happened error");
            break;
        }
        //检查标准输入
        if(FD_ISSET(STDIN_FILENO, &ready_set)){
            //从标准输入读，然后写到标准输出
            echostdin();
        }
        //检查serverfd
        if(FD_ISSET(serverfd, &ready_set)){
            struct sockaddr clientaddr;
            socklen_t socklen = sizeof(clientaddr);
            int clientfd = ::accept(serverfd, &clientaddr, &socklen);
            char ipport[100];
            get_ipport_str(&clientaddr, socklen, ipport, sizeof(ipport));
            printf("recv connect from client: %s\n", ipport);
            //为了简单，没有把clientfd加入到select中进行监听，而是直接echo处理完
            echo(clientfd, clientfd);
            ::close(clientfd);
            printf("close connect from client: %s\n", ipport);
        }
    }

    return 0;
}