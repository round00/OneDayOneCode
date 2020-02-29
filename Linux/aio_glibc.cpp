//
// Created by gjk on 2020/2/19.
//

#include <aio.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

const int SIGNO = SIGUSR1;
const int MAX_BUF = 128;

void signal_cb (int signo, siginfo_t *info, void * ctx){
    printf("signo=%d, info->signo=%d\n", signo, info->si_signo);
    //确保是规定的信号
    if(signo==SIGNO){
        struct aiocb* ptr = (struct aiocb*)info->si_value.sival_ptr;
        int ret = ::aio_error(ptr);
        printf("aio_error=%d\n",  ret);
        if(ret == 0){
            ret = ::aio_return(ptr);
            printf("aio_return=%d, read=%s\n",ret, (char*)ptr->aio_buf);
        }
    }
}
//信号通知
bool signal_aio(){
    //设置信号回调
    struct sigaction act;
    ::sigemptyset(&act.sa_mask);
    act.sa_sigaction = signal_cb;
    ::sigaction(SIGNO, &act, 0);  //这个相当于是将信号和回调绑定起来

    //构建aio请求
    struct aiocb cb;
    ::bzero(&cb, sizeof(cb));
    cb.aio_fildes = STDIN_FILENO;   //从标准输入读
    cb.aio_buf = malloc(MAX_BUF+1);
    cb.aio_nbytes = MAX_BUF;
    cb.aio_offset = 0;

    //设置通知方式
    cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;    //以信号来通知
    cb.aio_sigevent.sigev_signo = SIGNO;
    cb.aio_sigevent.sigev_value.sival_ptr = &cb;

    //提交异步读请求
    int ret = ::aio_read(&cb);
    if(ret==-1){
        fprintf(stderr, "aio_read failed\n");
        return false;
    }
    return true;
}

void thread_cb(sigval_t val){
    struct aiocb* ptr = (struct aiocb*)val.sival_ptr;
    int ret = ::aio_error(ptr);
    printf("aio_error=%d\n",  ret);
    if(ret == 0){
        ret = ::aio_return(ptr);
        printf("aio_return=%d, read=%s\n",ret, (char*)ptr->aio_buf);
    }
}
//线程通知，即回调函数
bool thread_aio(){
    //构建aio请求
    struct aiocb cb;
    ::bzero(&cb, sizeof(cb));
    cb.aio_fildes = STDIN_FILENO;   //从标准输入读
    cb.aio_buf = malloc(MAX_BUF+1);
    cb.aio_nbytes = MAX_BUF;
    cb.aio_offset = 0;

    //设置通知方式
    cb.aio_sigevent.sigev_notify = SIGEV_THREAD;    //以回调函数来通知
    cb.aio_sigevent.sigev_notify_function = thread_cb;
    cb.aio_sigevent.sigev_notify_attributes = NULL;
    cb.aio_sigevent.sigev_value.sival_ptr = &cb;

    //提交异步读请求
    int ret = ::aio_read(&cb);
    if(ret==-1){
        fprintf(stderr, "aio_read failed\n");
        return false;
    }
    return true;
}
int main()
{
    ::signal(SIGHUP, SIG_IGN);
    thread_aio();
    int n = 0;
    while(1){
//        printf("n=%d\n", n++);
        ::sleep(100);
        if(n>=100)break;
    }


    return 0;
}