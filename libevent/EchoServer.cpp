//
// Created by gjk on 2019/12/29.
//
#include <event.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <event2/listener.h>


static const int PORT = 9981;

static void read_cb(struct bufferevent *bev, void *ctx){
    fprintf(stderr, "At %s\n", __func__);
    struct evbuffer* input = bufferevent_get_input(bev);
    struct evbuffer* output = bufferevent_get_output(bev);

//    Move all data from one evbuffer into another evbuffer
    evbuffer_add_buffer(output, input);
}
void event_cb(struct bufferevent *bev, short events, void *ctx){
    fprintf(stderr, "At %s, events=%d\n", __func__, events);
    if(events & BEV_EVENT_ERROR){
        fprintf(stderr, "Error from bufferevent\n");
    }
    if(events & (BEV_EVENT_EOF|BEV_EVENT_ERROR)){
        bufferevent_free(bev);
    }

}

static void accpet_conn(struct evconnlistener *lev,
        evutil_socket_t fd, struct sockaddr *addr, int socklen, void *arg){
    fprintf(stderr, "There is a new connection.\n");
    struct event_base* base = evconnlistener_get_base(lev);
    struct bufferevent* bev = bufferevent_socket_new(
            base, fd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, read_cb, NULL, event_cb, NULL);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
}
static void accept_error(struct evconnlistener *lev, void * arg){
    struct event_base* base = evconnlistener_get_base(lev);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "At %s, got an error:%d(%s), exit. \n",
            __func__, err, evutil_socket_error_to_string(err));
    event_base_loopexit(base, NULL);
}
int main()
{
    struct event_base* base;
    struct evconnlistener* listener;
    struct sockaddr_in sin;

    //初始化一个event base
    base = event_base_new();
    if(!base){
        printf("event_base_new failed\n");
        return -1;
    }

    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = htonl(0);

    listener = evconnlistener_new_bind(base, accpet_conn, NULL,
                                       LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
                                       (struct sockaddr*)&sin, sizeof sin);
    if(!listener){
        printf("evconnlistener_new_bind failed\n");
        return -1;
    }

    //设置监听的错误回调
    evconnlistener_set_error_cb(listener, accept_error);

    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_base_free(base);

    printf("quit.\n");
    return 0;
}



