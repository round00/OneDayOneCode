//
// Created by gjk on 2019/12/29.
//
#include <event.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <event2/listener.h>


FILE* dumpfile = NULL;
void cb(int sock, short what, void* arg){
    struct event_base* base = (struct event_base*)arg;

    event_base_loopbreak(base);
}

void main_loop(struct event_base*base, evutil_socket_t watchdog_fd){
    struct event* watchdog_event = event_new(
            base, watchdog_fd, EV_READ, cb, base);
    event_add(watchdog_event, NULL);
    if(dumpfile){
        event_base_dump_events(base, dumpfile);
        ::fclose(dumpfile);
        dumpfile = NULL;
    }
    event_base_dispatch(base);
}

int initlistensock(short port){
    int lfd = ::socket(AF_INET, SOCK_STREAM, 0);
    fcntl(lfd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;

    if(::bind(lfd, (struct sockaddr*)&sin, sizeof sin) < 0){
        printf("%s bind error=%s\n", __func__, strerror(errno));
        return -1;
    }

    if(::listen(lfd, 20)<0){
        printf("%s listen error=%s\n", __func__, strerror(errno));
        return -1;
    }
    return lfd;
}

//修改事件的回调和回调参数，事件当前不能是未决状态
int replace_callback(struct event* ev, event_callback_fn new_cb, void* new_arg){
    int pending = event_pending(ev, EV_READ|EV_WRITE|EV_TIMEOUT|EV_SIGNAL, NULL);
    if(pending){
        printf("event is pending, change callback failed\n");
        return -1;
    }

    struct event_base* base;
    int fd;
    short what;
    event_get_assignment(ev, &base, &fd, &what, NULL,NULL);
    event_assign(ev, base, fd, what, new_cb, new_arg);
    return 0;
}

void evbuffer_test(){
    struct evbuffer* buf = evbuffer_new();
    printf("1. buf len=%ld \n", evbuffer_get_length(buf));
    evbuffer_add_printf(buf, "Hello, I'm %s", "gjk");
    printf("2. buf len=%ld \n", evbuffer_get_length(buf));
    char *buffer[128] = {0};
    int ret = evbuffer_remove(buf, (void*)buffer, 128);
    printf("3. ret=%d\n", ret);
    printf("4. buf len=%ld \n", evbuffer_get_length(buf));

    evbuffer_free(buf);
}

int main(){
    evbuffer_test();
    return 0;
    dumpfile = fopen("evdump", "w");
    if(!dumpfile){
        printf("file open failed\n");
        return 0;
    }

    int sfd = initlistensock(9981);
    struct event_base* base = event_base_new();
    if(!base){
        printf("event_base_new failed\n");
        return 0;
    }
    main_loop(base, sfd);

    printf("loop exit by exit: %d\n", event_base_got_exit(base));
    printf("loop exit by break: %d\n", event_base_got_break(base));
    return 0;
}