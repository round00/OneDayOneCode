//
// Created by gjk on 2019/12/29.
//

#include <event.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <event2/listener.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <evhttp.h>

static const char MYHTTP_SIGNATURE[] = "myhttpd v 0.0.1";

void http_handler(struct evhttp_request *req, void *arg){
    char output[2048] = {0};
    char tmp[1024];

    const char* uri;
    uri = evhttp_request_uri(req);
    sprintf(tmp, "uri=%s\n", uri);
    strcat(output, tmp);

    sprintf(tmp, "uri=%s\n", req->uri);
    strcat(output, tmp);

    char* decoded_uri;
    decoded_uri = evhttp_decode_uri(uri);
    sprintf(tmp, "decoded_uri=%s\n", decoded_uri);
    strcat(output, tmp);

    struct evkeyvalq params;
    evhttp_parse_query(decoded_uri, &params);
    sprintf(tmp, "q=%s\n", evhttp_find_header(&params, "q"));
    strcat(output, tmp);

    sprintf(tmp, "s=%s\n", evhttp_find_header(&params, "s"));
    strcat(output, tmp);

    free(decoded_uri);

    char *post_data = (char*)EVBUFFER_DATA(req->input_buffer);
    sprintf(tmp, "post_data=%s\n", post_data);
    strcat(output, tmp);

    evhttp_add_header(req->output_headers, "Servers", MYHTTP_SIGNATURE);
    evhttp_add_header(req->output_headers,
            "Content-Type", "text/plain; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");

    struct evbuffer* buf = evbuffer_new();
    evbuffer_add_printf(buf, "It works!\n%s\n", output);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}
void show_help(){
    char *help =
            "http://localhost:9981\n"
            "-l <ip_addr> interface to listen on, default is 0.0.0.0\n"
            "-p <num> port num to listen on, default is 9981\n"
            "-d run as a deamon\n"
            "-t <second> timeout for a http request, default is 120 seconds\n"
            "-h for help\n"
            "\n";
    fprintf(stderr, "%s", help);
}

void signal_handler(int sig){
    switch (sig){
        case SIGTERM:
        case SIGHUP:
        case SIGQUIT:
        case SIGINT:
            //之前写的都是event_base_loopbreak,按照文档上来说，
            // 这个方法不安全还是应该调用event_base_loopbreak
            event_loopbreak();
            break;
    }
}

int main(int argc, char* argv[]){
    ::signal(SIGHUP, signal_handler);
    ::signal(SIGTERM, signal_handler);
    ::signal(SIGQUIT, signal_handler);
    ::signal(SIGINT, signal_handler);

    char* httpd_option_listen = "0.0.0.0";
    int httpd_option_port = 9981;
    int httpd_option_daemon = 0;
    int httpd_option_timeout = 120; //second

    int c;
    while((c = ::getopt(argc, argv, "l:p:dt:h")) != -1){
        switch(c){
            case 'l':
                httpd_option_listen = optarg;
                break;
            case 'p':
                httpd_option_port = atoi(optarg);
                break;
            case 'd':
                httpd_option_daemon = 1;
                break;
            case 't':
                httpd_option_timeout = atoi(optarg);
                break;
            case 'h':
            default:
                show_help();
                exit(EXIT_SUCCESS);
        }
    }

    if(httpd_option_daemon){
        //真实的以守护进程方式运行比这要多处理一些东西
        pid_t pid = fork();
        if(pid<0){
            fprintf(stderr, "fork failed\n");
            exit(EXIT_FAILURE);
        }
        if(pid > 0){
            exit(EXIT_SUCCESS);
        }
    }

    event_init();

    struct evhttp* httpd;
    httpd = evhttp_start(httpd_option_listen, httpd_option_port);
    evhttp_set_timeout(httpd, httpd_option_timeout);

    evhttp_set_gencb(httpd, http_handler, NULL);

    event_dispatch();

    evhttp_free(httpd);
    fprintf(stderr, "myhttp quit. Bye~\n");
    return 0;
}