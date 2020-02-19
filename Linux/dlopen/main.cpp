//
// Created by gjk on 2020/2/18.
//
#include <stdio.h>
#include <dlfcn.h>

typedef int (*ADD)(int, int);
int main(){
    while(1){
        void * handle = ::dlopen("./libadd.so", RTLD_LAZY);
        if(!handle){
            fprintf(stderr, "dlopen failed, err=%s\n", ::dlerror());
            ::dlclose(handle);
            break;
        }
        fprintf(stdout, "libadd.so load success, err=%s\n", ::dlerror());
        ADD addFunc = (ADD)::dlsym(handle, "getAdd");
        if(!addFunc){
            fprintf(stderr, "dlsym load func failed, err=%s\n", ::dlerror());
            ::dlclose(handle);
            break;
        }
        fprintf(stdout, "function load success\n");
        fprintf(stdout, "Input two numbers: ");
        int a,b;scanf("%d%d", &a, &b);
        int res=addFunc(a, b);
        fprintf(stdout, "result=%d\n", res);
        ::dlclose(handle);
    }

    return 0;
}
