//
// Created by gjk on 2020/3/2.
//

#include <evhttp.h>
#include <event2/event.h>
#include <event2/thread.h>

void http_handler(struct evhttp_request *req, void *arg){
    struct evbuffer* buf = evbuffer_new();

    evbuffer_add_printf(buf, "Hello World");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}
void hello_handler(struct evhttp_request *req, void *arg) {
    printf("123");
}
int main(int argc, char* argv[]){
    event_use_pthreads();
    struct event_base* base = event_base_new();
    struct evhttp* http = evhttp_new(base);
    evhttp_set_cb(http, "/", http_handler, NULL);
    evhttp_set_gencb(http, http_handler, NULL);
    evhttp_bind_socket(http, "0.0.0.0", 9876);
    event_base_dispatch(base);

    return 0;
}