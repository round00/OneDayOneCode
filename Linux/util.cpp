//
// Created by gjk on 2020/2/28.
//

#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include "util.h"

int open_listenedfd(int port){
    struct addrinfo hints, *listp, *p;
    int listenfd = -1, optval = 1;

    memset(&hints, 0, sizeof(hints));
    //指定tcp类型
    hints.ai_socktype = SOCK_STREAM;
    //指定可以使用任意IP地址
    hints.ai_flags = AI_PASSIVE|AI_ADDRCONFIG;
    //使用数字的端口号
    hints.ai_flags |= AI_NUMERICSERV;
    //构造port的字符串
    char sport[11];
    sprintf(sport, "%d", port);
    //这个API既可以用于IPV4可以用于V6
    if(::getaddrinfo(NULL, sport, &hints, &listp) != 0){
        fprintf(stderr, "getaddrinfo failed, err=%s\n", strerror(errno));
        return listenfd;
    }
    //遍历返回的地址列表
    for(p = listp; p;p = p->ai_next){
        if((listenfd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0){
            continue;
        }
        if(::setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) != 0){
            fprintf(stderr, "setsockopt failed, err=%s\n", strerror(errno));
            continue;
        }
        //即使不设置成非阻塞的，IO多路复用函数select也能正常工作，所以为什么要设置呢
        setsock_nonblock(listenfd);

        if(::bind(listenfd, p->ai_addr, p->ai_addrlen) == 0){
            break;
        }
        ::close(listenfd);
    }
    ::freeaddrinfo(listp);
    if(!p){
        return -1;
    }

    if(::listen(listenfd, MAX_LISTEN_QUEUE) != 0){
        fprintf(stderr, "listen failed, err=%s\n", strerror(errno));
        ::close(listenfd);
        return -1;
    }
    return listenfd;
}

void get_ipport_str(struct sockaddr* addr, size_t socklen, char* buf, size_t n){

    char ip[MAX_IP_LEN], port[MAX_PORT_LEN];
    if(::getnameinfo(addr, socklen, ip, MAX_IP_LEN, port, MAX_PORT_LEN, 0) != 0){
        fprintf(stderr, "getnameinfo failed, err=%s\n", strerror(errno));
        return;
    }
    snprintf(buf, n, "%s:%s", ip, port);
}

void echo(int rfd, int wfd){
    if(rfd < 0 || wfd<0){
        fprintf(stderr, "fd is less than zero\n");
        return;
    }
    rio_t rio;
    rio_readbuf_init(&rio, rfd);
    ssize_t n;
    char buf[MAX_BUF_LEN];
    while((n = rio_readbufline(&rio, buf, MAX_BUF_LEN)) > 0){
        printf("recv: %s", buf);
        rio_write(wfd, buf, n);
    }
}

ssize_t rio_read(int fd, void* buf, size_t n){
    //buf剩余空间
    size_t left = n;
    ssize_t nread;
    //buf的迭代指针
    char * p = (char*)buf;
    while(left > 0){
        if((nread = ::read(fd, p, left)) < 0){
            //系统调用被打断，下次循环继续读就好
            if(errno == EINTR)nread = 0;
            //不是上面的那个错误，则是出错了，直接返回吧
            else {
                fprintf(stderr, "rio_readn failed, err=%s\n", strerror(errno));
                return -1;
            }
        }
        //读了0个字符，意思是读完了
        else if(nread == 0){
            break;
        }
        //更新buf的迭代器
        left -= nread;
        p += nread;
    }
    return n-left;
}


ssize_t rio_write(int fd, void* buf, size_t n){
    //buf未写字节数
    size_t left = n;
    ssize_t nwrite;
    //buf的迭代指针
    char * p = (char*)buf;
    while(left > 0){
        if((nwrite = ::write(fd, buf, left)) < 0){
            //系统调用被打断，下次循环继续写就好
            if(errno == EINTR)nwrite = 0;
            //不是上面的那个错误，则是出错了，直接返回吧
            else{
                fprintf(stderr, "rio_writen failed, err=%s\n", strerror(errno));
                return -1;
            }
        }
        //更新迭代器
        left -= nwrite;
        p += nwrite;
    }

    return n;
}

void rio_readbuf_init(rio_t * rp, int fd){
    if(!rp || fd<0){
        return;
    }
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_readbuf(rio_t *rp, void* buf, size_t n){
    if(!rp)return -1;
    int cnt;
    //缓冲区为空，先往缓冲区中读
    while(rp->rio_cnt<=0){
        //读内容到缓冲区中
        rp->rio_cnt = ::read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        //可能出错了
        if(rp->rio_cnt < 0){
            if(errno != EINTR){
                fprintf(stderr, "rio_readbuf failed, err=%s\n", strerror(errno));
                return -1;
            }
        }
        //读完了(EOF)
        else if(rp->rio_cnt == 0){
            return 0;
        }
        //缓冲区中有数据了
        else{
            rp->rio_bufptr = rp->rio_buf;
        }
    }
    //计算出应该拷贝多少字节=min(rp->cnt, n)
    cnt = n;
    if(rp->rio_cnt < n){
        cnt = rp->rio_cnt;
    }
    //拷贝具体内容
    memcpy(buf, rp->rio_bufptr, cnt);
    //更新迭代器
    rp->rio_cnt -= cnt;
    rp->rio_bufptr += cnt;
    return cnt;
}

ssize_t rio_readbufn(rio_t *rp, void* buf, size_t n){
    //buf剩余空间
    size_t left = n;
    ssize_t nread;
    //buf的迭代指针
    char * p = (char*)buf;
    while(left > 0){
        if((nread = rio_readbuf(rp, p, left)) < 0){
            fprintf(stderr, "rio_readn failed, err=%s\n", strerror(errno));
            return -1;
        }
        //读了0个字符，意思是读完了
        else if(nread == 0){
            break;
        }
        //更新buf的迭代器
        left -= nread;
        p += nread;
    }
    return n-left;
}

ssize_t rio_readbufline(rio_t *rp, void* buf, size_t n){
    int i, nread;
    char c, *p = (char*)buf;
    //这里是i=1
    for(i = 1; i<n; ++i){
        if((nread = rio_readbuf(rp, &c, 1)) == 1){
            *p++ = c;
            if(c == '\n'){
                i++;
                break;
            }
        }else if(nread == 0){
            if(i==1){   //第一个字节就没读到，那说明就是EOF了
                return 0;
            }else {     //读着读着就没了，就是读完了
                break;
            }
        }else{//这里返回小于0的值，肯定是出错了
            return -1;
        }
    }
    //添加\0
    *p = 0;
    return i-1;
}

int setsock_nonblock(int fd){
    int f = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, f|O_NONBLOCK);
}
