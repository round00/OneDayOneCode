//
// Created by gjk on 2020/1/14.
//
#include <stdio.h>
#include <exception>
#include <stdexcept>
#include <typeinfo>
class PlayException : public std::exception{
public:
    PlayException(int v):val(v){}
    int val;
};
void fun() noexcept(false) {
    throw PlayException(2);
}

class A{
    virtual void fun()const noexcept = 0;
};


int main(){
    typedef decltype(fun) *p;
    double x;
    printf("%s\n", typeid(PlayException).name());
    try {
        fun();
    }catch(PlayException e){
        printf("errcode = %s\n", e.what());
    }
    std::exception e;
    std::bad_alloc ba;
    std::runtime_error rt("");
//    std::terminate();

    return 0;
}