//
// Created by gjk on 2019/12/29.
//
#include <event.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <errno.h>
#include <event2/listener.h>

//static 会将作用域限制在当前文件内
static const char MESSAGE[] = "Hello, World\n";
static const int PORT = 9981;

static void listener_cb(struct evconnlistener*, evutil_socket_t,
        struct sockaddr*, int socklen, void*);
static void conn_writecb(struct bufferevent*, void*);
static void conn_eventcb(struct bufferevent*, short,void*);
static void signal_cb(evutil_socket_t, short, void*);

int main()
{
    struct event_base* base;
    struct evconnlistener* listener;
    struct event* signal_event;
    struct sockaddr_in sin;

    //初始化一个event base
    base = event_base_new();
    if(!base){
        printf("event_base_new failed\n");
        return -1;
    }

    //创建链接监听器
    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
//    sin.sin_addr.s_addr = INADDR_ANY;
    listener = evconnlistener_new_bind(base, listener_cb, (void*)base,
            LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
            (struct sockaddr*)&sin, sizeof sin);
    if(!listener){
        printf("evconnlistener_new_bind failed\n");
        return -1;
    }

    //创建signal event，并添加事件
    signal_event = evsignal_new(base, SIGINT, signal_cb, (void*)base);
    if(!signal_event || event_add(signal_event, NULL)<0){
        printf("evsignal_new failed\n");
        return -1;
    }
    //开启事件分发，event_base_dispatch就相当于是没有设置flag的event_base_loop
    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);

    printf("quit.\n");
    return 0;
}

static void listener_cb(struct evconnlistener* listener, evutil_socket_t fd,
                        struct sockaddr* sa, int socklen, void* arg){
    struct event_base* base = (struct event_base*)arg;
    //这是在对客户端的连接进行操作
    struct bufferevent* bev = bufferevent_socket_new(
            base, fd, BEV_OPT_CLOSE_ON_FREE);
    if(!bev){
        printf("bufferevent_socket_new failed\n");
        event_base_loopbreak(base);
        return ;
    }
    //新创建的bufferevent 默认写入是启用的，但是读取没有启用
    bufferevent_setcb(bev, NULL, conn_writecb, conn_eventcb, NULL);
    bufferevent_enable(bev, EV_WRITE);
    bufferevent_disable(bev, EV_READ);
    bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

static void conn_writecb(struct bufferevent* bev, void*){
    struct evbuffer* out = bufferevent_get_output(bev);
    printf("At %s, evbuffer=%p\n", __func__, out);
    if(evbuffer_get_length(out) == 0){
        printf("flushed answer\n");
        bufferevent_free(bev);
    }
}
static void conn_eventcb(struct bufferevent*bev, short events, void* arg){
    printf("At %s: ", __func__);
    if(events & BEV_EVENT_EOF){
        printf("Connection closed\n");
    }else if(events & BEV_EVENT_ERROR){
        printf("Got an error on the connection: %s\n", strerror(errno));
    }else{
        printf("events=%d\n", events);
    }
    bufferevent_free(bev);
}

static void signal_cb(evutil_socket_t sig, short event, void*arg){
    struct event_base* base = (struct event_base*)arg;
    //这种写法感觉就很爽
    struct timeval delay = {2, 0};
    printf("Got an interrupt signal, exit in 2 seconds.\n");
    event_base_loopexit(base, &delay);
}