//
// Created by gjk on 2020/3/9.
//
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <paths.h>

void daemon_run(){
    pid_t pid = fork();
    //加上这个printf后，运行daemon后子进程会卡一下，需要输入一个回车才能继续运行
//    printf("pid=%d\n", pid);
    if(pid < 0){
        perror("fork error\n");
        exit(-1);
    }else if(pid>0){
        printf("parent quit\n");
        exit(0);
    }

    //1、成为组长进程，摆脱终端
    if(::setsid() < 0){
        perror("setsid failed\n");
        exit(0);
    }
    //2、修改文件权限
    ::umask(0);
    //3、修改工作目录
    if(::chdir("/") < 0){
        perror("chdir failed\n");
        exit(0);
    }
    //4、关闭0、1、2描述符和父进程的文件描述符文件
    struct rlimit rl;   //先获取进程当前的最大描述符
    if(::getrlimit(RLIMIT_NOFILE, &rl) < 0){
        perror("getrlimit");
        exit(0);
    }

    if(rl.rlim_max == RLIM_INFINITY){
        rl.rlim_max = 1024;
    }
    for(int i = 0;i<rl.rlim_max; ++i){
        ::close(i);
    }
}

int main(){
    daemon_run();
    while(1){
        ::sleep(2);
    }
    return 0;
}