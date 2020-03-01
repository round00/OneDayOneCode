//
// Created by gjk on 2020/3/1.
// 验证多线程不加锁会出错的情况
// 最后运行结果应该是50000000
// 不加同步操作：35321370,执行时间0.116s
// 加同步操作：50000000，执行时间2.754s
//

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

volatile int cnt = 0;
const int MAX_THREAD_COUNT = 5;
//用信号量来同步
sem_t mutex;
void * thread_func(void* arg){
    int n = *(int*)arg;
    for(int i = 0;i<n;++i){
        sem_wait(&mutex);
        cnt = cnt+1;
        sem_post(&mutex);
    }

    return NULL;
}

int main(){

    sem_init(&mutex, 0, 1);
    pthread_t pids[MAX_THREAD_COUNT];
    int ITER = 10000000;
    for(int i = 0;i<MAX_THREAD_COUNT; ++i){
        ::pthread_create(&pids[i], NULL, thread_func, &ITER);
    }
    for(int i = 0;i<MAX_THREAD_COUNT; ++i){
        ::pthread_join(pids[i], NULL);
    }
    printf("%d\n", cnt);
    return 0;
}
