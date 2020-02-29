//
// Created by gjk on 2020/2/28.
//

#ifndef ONEDAYONECODE_UTIL_H
#define ONEDAYONECODE_UTIL_H

#include <sys/types.h>
const int MAX_LISTEN_QUEUE = 1024;
//成功返回可用的TCP服务器监听port端口的socket fd，失败返回-1
int open_listenedfd(int port);

//从sockaddr生成IP:PORT字符串
struct sockaddr;
const int MAX_IP_LEN = 50;
const int MAX_PORT_LEN = 10;
void get_ipport_str(struct sockaddr* addr, size_t socklen, char* buf, size_t n);

//从rfd每读一行数据，就原样输出到wfd一行
const int MAX_BUF_LEN = 256;
void echo(int rfd, int wfd);


//==================RIO系列IO函数==================
//来自于CSAPP Chapter10
//从fd读取内容，buf的最大空间是n，成功返回读取字节数，失败返回-1
ssize_t rio_read(int fd, void* buf, size_t n);
//向fd写入内容，buf中的数据量是n，成功返回写入的字节数，失败返回-1
ssize_t rio_write(int fd, void* buf, size_t n);

const int RIO_BUFSIZE = 8192;
//用于带缓冲的RIO读取函数的数据结构
typedef struct{
    //rio缓冲服务的fd
    int     rio_fd;
    //rio缓冲中未读的字节数
    int     rio_cnt;
    //rio缓冲当前该读的位置
    char*   rio_bufptr;
    //rio缓冲数据存放的数组
    char    rio_buf[RIO_BUFSIZE];
}rio_t;
//初始化带缓冲的读操作，要使用带缓冲读之前，必须调用这个，它会先和fd绑定到一起
void rio_readbuf_init(rio_t * rp, int fd);
//从缓冲区中读取数据，最多读取n个字节，成功返回读取的数量，失败返回-1
ssize_t rio_readbufn(rio_t *rp, void* buf, size_t n);
//从缓冲区中读取一行数据，最多读取n个字节，成功返回读取的数量，失败返回-1
ssize_t rio_readbufline(rio_t *rp, void* buf, size_t n);

//==================RIO系列IO函数==================


int setsock_nonblock(int fd);
#endif //ONEDAYONECODE_UTIL_H
