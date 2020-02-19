//
// Created by gjk on 2019/12/28.
//
#include <event.h>
#include <unistd.h>

int main(){
    //查看libevent的版本号
    printf("libevent version=%s, supported methods are:\n",event_get_version());
    //查看libevent支持的后台方法
    const char** method = event_get_supported_methods();
    for(int i = 0;method[i]; ++i){
        printf("%s\n", method[i]);
    }
    puts("");

    //创建一个event_base
    struct event_base* base = event_base_new();
    if(!base){
        printf("event_base_new failed\n");
        return 0;
    }

    //设置base的优先级个数,第二个参数不能超过EVENT_MAX_PRIORITIES
    event_base_priority_init(base, EVENT_MAX_PRIORITIES);

    //查看event_base正在使用的后台方法
    printf("event base's backend method is=%s\n\n", event_base_get_method(base));
    //查看支持的特征
    printf("supported features:\n");
    int f = event_base_get_features(base);
    if ((f & EV_FEATURE_ET))
        printf(" Edge-triggered events are supported.\n");
    if ((f & EV_FEATURE_O1))
        printf(" O(1) event notification is supported.\n");
    if ((f & EV_FEATURE_FDS))
        printf(" All FD types are supported.\n");
    puts("");

    //fork一个新进程后，要想在新进程中继续使用base，要调用下面这个
//    pid_t pid = fork();
//    if(pid!=0){
//        //parent_continue();
//    } else{
//        if(event_reinit(base)==-1){
//            printf("event_reinit failed\n");
//            return 0;
//        }
//        //child_continue();
//    }


    //释放掉event_base，这个只是释放event_base，并不会释放
    //和它相关的资源，例如套接字、指针什么的。
    event_base_free(base);
    return 0;
}