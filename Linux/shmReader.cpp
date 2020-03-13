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
    //创建共享内存的标识符,这里认为是已经创建好了，直接获取就行
    int shmid = ::shmget(key, SIZE, IPC_CREAT);
    if(shmid<0){
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
    while(i<SIZE/2){
        printf("%s\n", buffer);
        i++;
        ::sleep(1);
    }
    //取消挂接
    ::shmdt(buffer);
    //这里只取消挂接就好了，删除的任务交给创建者执行
    return 0;
}