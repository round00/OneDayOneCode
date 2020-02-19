//
// Created by gjk on 2020/1/29.
//
//模板元编程
#include <stdio.h>

//在编译期计算阶乘,TMP界的hello world
template <unsigned n>
struct Factorial{
    enum {
        value = n * Factorial<n-1>::value
    };
};
template <>
struct Factorial<0>{
    enum {
        value = 1
    };
};

int main()
{

    printf("%d\n", Factorial<5>::value);
    return 0;
}