//
// Created by gjk on 2019/12/28.
//
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <event.h>
#include <time.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

const int MAX_EVENTS = 1024;
const int BUFLEN = 128;
const int SERVERPORT = 9981;
void to_upper(char* s){
    while(*s){
        if(*s>='a'&&*s<='z'){
            *s = *s - 'a' + 'A';
        }
        s++;
    }
}
struct myevent{
    int fd;
    int events;
    void* arg;
    void (*call_back)(int fd, int events, void *arg);
    int status;     //1监听事件中，0没有
    char buf[BUFLEN];
    int len;
    long last_active;
};

int g_efd;
struct myevent g_events[MAX_EVENTS+1];

void event_set(struct myevent*ev, int fd, void(*callback)(int,int,void*), void*arg){
    ev->fd = fd;
    ev->call_back = callback;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    ev->last_active  = time(NULL);
}

void epoll_add(int efd, int events, struct myevent *ev){
    struct epoll_event epv = {0, {0}};
    int op;
    epv.data.ptr = ev;
    epv.events = ev->events = events;
    if(ev->status == 1){
        op = EPOLL_CTL_MOD;
    }else{
        op = EPOLL_CTL_ADD;
        ev->status = 1;
    }
    if(::epoll_ctl(efd, op, ev->fd, &epv)<0){
        printf("event add failed [fd=%d], events[%d], err=%d\n", ev->fd, events, errno);
    }else{
        printf("event add ok [fd=%d], events[%d]\n", ev->fd, events);
    }
}

void epoll_del(int efd, struct myevent* ev){
    struct epoll_event epv = {0, {0}};
    if(ev->status == 0)
        return;

    epv.data.ptr = ev;
    ev->status = 0;
    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &epv);
}

void recv_data(int fd, int events, void*arg);

void send_data(int fd, int events, void*arg){
    struct myevent* ev = (struct myevent*)arg;
    to_upper(ev->buf);
    int len = send(fd, ev->buf, ev->len, 0);

    epoll_del(g_efd, ev);
    if(len > 0){
        printf("send[fd=%d], %s\n", fd, ev->buf);
        event_set(ev, fd, recv_data, ev);
        epoll_add(g_efd, EPOLLIN, ev);
    }else{
        ::close(ev->fd);
        printf("send[fd=%d] err=%s\n", ev->fd, strerror(errno));
    }
}

void recv_data(int fd, int events, void*arg){
    struct myevent* ev = (struct myevent*)arg;
    int len = ::recv(fd, ev->buf, sizeof ev->buf, 0);

    epoll_del(g_efd, ev);
    if(len > 0){
        ev->len = len;
        ev->buf[len] = 0;
        printf("recv[%d]: %s\n", fd, ev->buf);

        event_set(ev, fd, send_data, ev);
        epoll_add(g_efd, EPOLLOUT, ev);
    }else if(len==0){
        ::close(ev->fd);
        printf("fd=%d, pos[%d] closed\n", fd, (int)(ev-g_events));
    } else{
        ::close(fd);
        printf("recv[fd=%d] err=%s\n", fd, strerror(errno));
    }
}

void acceptconn(int lfd, int events, void*age){
    struct sockaddr_in cin;
    socklen_t len = sizeof cin;
    int cfd;
    if((cfd = ::accept(lfd, (struct sockaddr*)&cin, &len)) < 0){
        if(errno!=EAGAIN && errno!=EINTR){

        }
        printf("%s accept, %s\n", __func__, strerror(errno));
        return;
    }
    int i;
    do{
        for(i = 0;i<MAX_EVENTS; ++i){
            if(g_events[i].status == 0)
                break;
        }

        if(i == MAX_EVENTS){
            printf("%s: max connect limit [%d]\n",__func__, MAX_EVENTS);
            break;
        }

        int flag = 0;
        if((flag = ::fcntl(cfd, F_SETFL, O_NONBLOCK)) < 0){
            printf("%s: fcntl nonblocking failed, %s\n", __func__, strerror(errno));
            break;
        }

        event_set(&g_events[i], cfd, recv_data, &g_events[i]);
        epoll_add(g_efd, EPOLLIN, &g_events[i]);
    }while(false);

    printf("new connection [%s:%d][time:%ld], pos[%d]\n",
            inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), g_events[i].last_active, i);
}

void initlistensock(int efd, short port){
    int lfd = ::socket(AF_INET, SOCK_STREAM, 0);
    fcntl(lfd, F_SETFL, O_NONBLOCK);

    event_set(&g_events[MAX_EVENTS], lfd, acceptconn, &g_events[MAX_EVENTS]);
    epoll_add(efd, EPOLLIN, &g_events[MAX_EVENTS]);

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;

    if(::bind(lfd, (struct sockaddr*)&sin, sizeof sin) < 0){
        printf("%s bind error=%s\n", __func__, strerror(errno));
        return;
    }

    if(::listen(lfd, 20)<0){
        printf("%s listen error=%s\n", __func__, strerror(errno));
        return;
    }
}


int main(){
    unsigned short port = SERVERPORT;
    g_efd = ::epoll_create(MAX_EVENTS+1);
    if(g_efd < 0){
        printf("epoll_create failed, err=%s\n", strerror(errno));
        return -1;
    }

    initlistensock(g_efd, port);

    struct epoll_event events[MAX_EVENTS+1];
    printf("server running on:port[%d]\n", port);
    int checkpos = 0, i;
    while(1){
        long now = time(NULL);
        for(i = 0;i<100; ++i, checkpos++){
            if(checkpos==MAX_EVENTS)checkpos = 0;

            if(g_events[checkpos].status!=1)
                continue;

            long duration = now - g_events[checkpos].last_active;
            if(duration >= 60){
                ::close(g_events[checkpos].fd);
                printf("fd=%d timeout\n", g_events[checkpos].fd);
                epoll_del(g_efd, &g_events[checkpos]);
            }
        }

        int nfd = ::epoll_wait(g_efd, events, MAX_EVENTS+1, 1000);
        if(nfd < 0){
            printf("epoll_wait failed, err=%s\n", strerror(errno));
            break;
        }

        for(i = 0;i<nfd; ++i){
            struct myevent* ev = (struct myevent*)events[i].data.ptr;
            if(events[i].events & EPOLLIN && ev->events & EPOLLIN){
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
            if(events[i].events & EPOLLOUT && ev->events & EPOLLOUT){
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
        }

    }


    return 0;
}