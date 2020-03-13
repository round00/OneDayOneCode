//
// Created by gjk on 2020/3/13.
// System V 共享内存demo
//

#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

const char* PATH = ".";
const int PROJID = 0x6666;
const int SIZE = 16;
int main(){
    //创建key，当作对shm的索引
    key_t key = ::ftok(PATH, PROJID);
    if(key<0){
        perror("ftok failed\n");
        return -1;
    }
    //ipcs -a可以显示IPC中的消息队列、共享内存、信号量的信息
    //这里shmget遇到了errno=17(File Exists)的错误，原因是之前创建shmid成功，但是没有删掉。通过ipcrm -m shmid命令可以手动删除
    //创建共享内存的标识符，0666是权限，不能少，少了无法访问
    int shmid = ::shmget(key, SIZE, IPC_CREAT|IPC_EXCL|0666);
    if(shmid<0){
        printf("error=%d\n", errno);
        perror("shmget failed\n");
        return -2;
    }
    //将shmid和共享内存进行挂接，共享内存是将物理内存映射到虚拟内存空间中。NULL表示由系统选择合适的地址
    char* buffer = (char*)::shmat(shmid, NULL, 0);
    if(!buffer){
        perror("shmat failed\n");
        return -3;
    }
    int i = 0;
    while(i<SIZE-1){
        buffer[i++] = 'A' + i%26;
        buffer[i] = 0;
        ::sleep(1);
    }
    //取消挂接
    ::shmdt(buffer);
    //删除shmid
    if(::shmctl(shmid, IPC_RMID, NULL) < 0){
        perror("shmctl failed\n");
        return -4;
    }

    return 0;
}