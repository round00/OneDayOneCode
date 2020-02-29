//
// Created by gjk on 2020/2/20.
//

// 这份代码展示一些gnu c的__attribute__的用法
// 可以设置函数属性、变量属性、类型属性，放于声明式的尾部 ';' 之前就行
// 更多属性可以查官网文档：http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Function-Attributes.html
#include <iostream>
using namespace std;



//============aligned============
//使编译器尽量保证8字节对齐
struct S{
    short x;
}__attribute__((aligned(8)));
typedef int int32_t __attribute__((aligned(8)));

//函数属性通过-Wall编译选项来打开警告
//进行检查函数参数是否满足格式化的要求，否则编译输出警告
void print(const char*format, ...)__attribute__((format(printf, 1,2)));
//通知编译器函数从不返回值
void func()__attribute__((noreturn));

//在main函数前后执行
void before_main()__attribute__((constructor));
void after_main()__attribute__((destructor));

int main(){
    cout<<sizeof(S)<<endl;
    cout<<sizeof(int32_t)<<endl;
    return 0;
}