//
// Created by gjk on 2020/2/28.
// 普通的echo服务器，阻塞的
//

#include "util.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

const int PORT = 9876;

int main(){

    int serverfd = open_listenedfd(PORT, 0);

    while(true){
        struct sockaddr clientaddr;
        socklen_t socklen = sizeof(clientaddr);
        int clientfd = ::accept(serverfd, &clientaddr, &socklen);
        if(clientfd <= 0){
            break;
        }
        char ipport[100];
        get_ipport_str(&clientaddr, socklen, ipport, sizeof(ipport));
        printf("recv connect from client: %s\n", ipport);
        echo(clientfd, clientfd);
        ::close(clientfd);
        printf("close connect from client: %s\n", ipport);
    }

    return 0;
}