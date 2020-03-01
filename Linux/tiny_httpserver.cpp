//
// Created by gjk on 2020/3/1.
// 一个简陋的web服务器，但是包含了太多值得学习的东西
//

#include "util.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

const int PORT = 9876;
const int MAX_LINE = 4096;
const char* basedir = "/tmp/httpserver";
//返回错误给客户端的通用接口
void clienterror(int fd, const char* method, const char* errcode,
        const char* shortmsg, const char* longmsg){
    char buf[MAX_LINE], body[MAX_LINE];
    //构造返回主体
    sprintf(body, "<html><title>KYHttpServer Error</title>");
    sprintf(body, "%s<body bgcolor=ffffff>\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errcode, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, method);
    sprintf(body, "%s<hr><em>KYHttpServer</em>\r\n", body);
    //响应状态行
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errcode, shortmsg);
    rio_write(fd, buf, strlen(buf));
    //响应类型
    sprintf(buf, "Content-type: text/html\r\n");
    rio_write(fd, buf, strlen(buf));
    //这里有空行表示响应头的结束
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    rio_write(fd, buf, strlen(buf));
    rio_write(fd, body, strlen(body));
}
//读取请求头，只是简单的打印，并不会进行处理
void read_request_headers(rio_t* rio){
    char buf[MAX_LINE];
    do{
        rio_readbufline(rio, buf, MAX_LINE);
        printf("%s", buf);
    }while(strcmp(buf, "\r\n") != 0);
}
//解析uri，如果是请求的是静态文件，返回1，动态返回0
int parse_uri(char* uri, char* filename, char* args){
    if(!strstr(uri, "cgi-bin")){    //静态文件
        if(args)args[0] = 0;
        strcpy(filename, basedir);
        strcat(filename, uri);
        if(uri[strlen(uri)-1] == '/'){
            strcat(filename, "index.html");
        }
        return 1;
    }else{  //动态文件
        char *p = index(uri, '?');//验证uri上是否有参数
        if(p){
            strcpy(args, p+1);
            *p = 0;
        }else{  //没有参数
            *args = 0;
        }
        strcpy(filename, basedir);
        strcpy(filename, uri);
    }
    return 0;
}

char* get_file_type(const char*filename){
    if(strstr(filename, ".html")){
        return "text/html";
    }else if(strstr(filename, ".gif")){
        return "image/gif";
    }else if(strstr(filename, ".png")){
        return "image/png";
    }else if(strstr(filename, ".jpg")){
        return "image/jpeg";
    }else if(strstr(filename, ".ico")){
        return "image/ico";
    }else{
        return "text/plain";
    }
}

void serve_static(int fd, const char* filename, int filesize){
    char* filetype = get_file_type(filename);
    char buf[MAX_LINE];
    //构造响应头
    sprintf(buf, "HTTP/1.1 200 OK\r\n");
    sprintf(buf, "%sServer: KYHttpServer\r\n", buf);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    //先把响应头发回去
    rio_write(fd, buf, strlen(buf));
    printf("Response headers: %s", buf);

    //发送响应主体,通过内存映射来直接将文件的内容发送出去
    int ffd = ::open(filename, O_RDONLY, 0);
    char* pfile = (char*)::mmap(0, filesize, PROT_READ, MAP_PRIVATE, ffd, 0);
    ::close(ffd);
    rio_write(fd, pfile, filesize);
    ::munmap(pfile, filesize);
}

void serve_dynamic(int fd, const char* filename, char* args){

}

//处理客户端连接上的http请求
void process(int clientfd){
    //初始化rio，利用它的读取一行的功能
    rio_t rio;
    rio_readbuf_init(&rio, clientfd);
    char buf[MAX_LINE];
    //读取第一行，第一行是请求行
    rio_readbufline(&rio, buf, MAX_LINE);
    printf("Request header = %s\n", buf);
    //读出请求行的方法、uri、和http版本
    char method[MAX_LINE], uri[MAX_LINE], version[MAX_LINE];
    sscanf(buf, "%s%s%s", method, uri, version);
    //不是GET请求就返回错误

    if(strcasecmp(method, "GET")!=0){
        clienterror(clientfd, method, "501", "Not implemented", "This server don't support your");
        return ;
    }
    //读取请求头
    read_request_headers(&rio);
    //解析请求的uri
    char filename[MAX_LINE], args[MAX_LINE];
    int bstatic = parse_uri(uri, filename, args);
    //验证请求的文件是否存在
    struct stat filestat;
    if(::stat(filename, &filestat) < 0){
        clienterror(clientfd, method, "404", "Not found", "Don't found this file");
        return;
    }
    if(bstatic){
        //要是普通文件且有读权限才行
        if(!S_ISREG(filestat.st_mode) || !(filestat.st_mode & S_IRUSR)){
            clienterror(clientfd, method, "403", "Forbidden", "Can't read this file");
            return;
        }
        serve_static(clientfd, filename, filestat.st_size);
    }else{
        //要是普通文件且有读权限才行
        if(!S_ISREG(filestat.st_mode) || !(filestat.st_mode & S_IXUSR)){
            clienterror(clientfd, method, "403", "Forbidden", "Can't run this program");
            return;
        }
        serve_dynamic(clientfd, filename, args);
    }
}

int main()
{
    int serverfd = open_listenedfd(PORT, 0);
    if(serverfd<0){
        fprintf(stderr, "serverfd less than zero\n");
        return 0;
    }

    while(true){
        int clientfd = ::accept(serverfd, NULL, NULL);
        if(clientfd<0)break;
        process(clientfd);
        ::close(clientfd);
    }
    return 0;
}