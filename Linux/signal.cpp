//
// Created by gjk on 2020/2/20.
//
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
void sig_recv(int signo){
    printf("recv %d, pid=%lu\n", signo, pthread_self());
    switch(signo){
        case SIGHUP:
            printf("SIGHUP\n");
            break;
        case SIGINT:    //Ctrl+C
            printf("SIGINT\n");
            break;
        case SIGQUIT:   //Ctrl+、
            printf("SIGQUIT\n");
            break;
        case SIGILL:
            printf("SIGILL\n");
            break;
        case SIGTRAP:
            printf("SIGTRAP\n");
            break;
        case SIGABRT:
            printf("SIGABRT\n");
            break;
        case SIGBUS:
            printf("SIGBUS\n");
            break;
        case SIGFPE:
            printf("SIGFPE\n");
            break;
        case SIGKILL:
            printf("SIGKILL\n");
            break;
        case SIGUSR1:
            printf("SIGUSR1\n");
            break;
        case SIGSEGV:
            printf("SIGSEGV\n");
            break;
        case SIGUSR2:
            printf("SIGUSR2\n");
            break;
        case SIGPIPE:
            printf("SIGPIPE\n");
            break;
        case SIGALRM:   //::alarm()
            printf("SIGALRM\n");
            break;
        case SIGTERM:
            printf("SIGTERM\n");
            break;
        case SIGSTKFLT:
            printf("SIGSTKFLT\n");
            break;
        case SIGCHLD:
            printf("SIGCHLD\n");
            break;
        case SIGCONT:
            printf("SIGCONT\n");
            break;
        case SIGSTOP:
            printf("SIGSTOP\n");
            break;
        case SIGTSTP:   //Ctrl+Z
            printf("SIGTSTP\n");
            break;
        case SIGTTIN:
            printf("SIGTTIN\n");
            break;
        case SIGTTOU:
            printf("SIGTTOU\n");
            break;
        case SIGURG:
            printf("SIGBUS\n");
            break;
        case SIGXCPU:
            printf("SIGXCPU\n");
            break;
        case SIGXFSZ:
            printf("SIGXFSZ\n");
            break;
        case SIGVTALRM:
            printf("SIGVTALRM\n");
            break;
        case SIGPROF:
            printf("SIGPROF\n");
            break;
        case SIGWINCH:
            printf("SIGWINCH\n");
            break;
        case SIGPOLL:
            printf("SIGPOLL\n");
            break;
        case SIGPWR:
            printf("SIGPWR\n");
            break;
        case SIGSYS:
            printf("SIGSYS\n");
            break;
        default:
            printf("\n");
            break;
    }
}

int main()
{
    sig_atomic_t;
    /*
     * 产生信号的方式：
     * 1、终端 kill -i pid，系统调用也有kill
     * 2、程序本身raise();
     * 3、alarm、abort、等函数调用产生特定信号
     * 4、组合按键例如Ctrl+C=SIGINT,Ctrl+\SIGQUIT,Ctrl+Z=SIGSTP
     * 5、硬件异常：例如除零什么的
     * */
    /*
     * 程序对信号的三种处理方式：
     * 1、SIG_IGN 忽略信号
     * 2、SIG_DFL 采取默认行为
     * 3、为特定信号设置handler来进行自定义处理
     * 4、通过sigmask系列函数设置屏蔽、阻塞信号等操作
     */
    /*!!!只有当从内核态返回用户态的时候才会处理信号，
     * 所以如果根本就不进行系统调用，是不会有信号的回调的*/
    printf("main thread id=%lu\n", pthread_self());
    for(int i = 1;i<=64; ++i){
        ::signal(i, sig_recv);
    }
    int n = 0;
    while(true){
        ::pause();      //阻塞一直到有信号产生
        ++n;
        if(n==20)break;
    }
    return 0;
}