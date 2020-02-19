//
// Created by gjk on 2020/2/8.
//
#include <stdio.h>
#include <mutex>    //在我的windows上这个里面并没有mutex类，要放到linux上跑才行

//=================懒汉模式=================
class Singleton1{
public:
    Singleton1(const Singleton1&) = delete;
    Singleton1& operator=(const Singleton1&) = delete;
    static Singleton1* getInstance(){
        if(p== nullptr){
            std::lock_guard<std::mutex> lock(mtx);
            if(p==nullptr){
                p = new Singleton1();
            }
        }
        return p;
    }

private:
    static Singleton1 *p;
    static std::mutex mtx;
    Singleton1()= default;
    ~Singleton1()= default;
};
Singleton1* Singleton1::p = nullptr;
std::mutex Singleton1::mtx;
//==========================================

//=================饿汉模式=================
class Singleton2{
public:
    Singleton2(const Singleton2&) = delete;
    Singleton2& operator=(const Singleton2&) = delete;

    static Singleton2* getInstance(){
        return p;
    }

private:
    static Singleton2 *p;
    Singleton2()= default;
    ~Singleton2()= default;
};
Singleton2* Singleton2::p = new Singleton2();
//==========================================

//=================内部变量=================
class Singleton3{
public:
    Singleton3(const Singleton3&) = delete;
    Singleton3& operator=(const Singleton3&) = delete;

    static Singleton3* getInstance(){
        static Singleton3 inst;
        return &inst;
    }

private:
    Singleton3()= default;
    ~Singleton3()= default;
};
//==========================================


int main()
{
    auto p1 = Singleton3::getInstance();
    auto p2 = Singleton3::getInstance();
    if(p1 == p2)printf("same\n");
    else printf("different\n");
    return 0;
}