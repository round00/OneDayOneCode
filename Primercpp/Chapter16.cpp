//
// Created by gjk on 2020/1/24.
//

#include <string.h>
#include <stdio.h>
#include <type_traits>
#include "Blob.h"

template <typename T>
int compare(const T& a, const T& b){
    if(a<b)return -1;
    if(a>b)return 1;
    return 0;
}

template <typename T>
T foo(T* p){
    T tmp = *p;
    return tmp;
}

template <typename T, class U> int calc(const T&, const U&);

template <typename T> inline T min(const T&, const T&);
//wrong: inline template <typename T> T min(const T&, const T&);

template <int N, int M>
int compare(const char (&a)[N], const char (&b)[M]){
    printf("N=%d M=%d\n", N, M);
    return strcmp(a, b);
}

template <typename T, typename F = std::less<T>>
int compare(const T& a, const T& b, F f = F()){
    if(f(a, b))return -1;
    if(f(b, a))return 1;
    return 0;
}

//不定返回值类型。
template <typename It>
auto f(It b, It e) -> decltype(*b)
{
    return *b;
}


int main()
{
    char s[12] = {0};
    printf("%d\n", compare(s, "hello"));

    Blob<int> b = {0,1,2,3,4};
    for(Blob<int>::size_type i = 0; i<b.size(); ++i){
        b[i] = i*i;
        printf("%d\n", b[i]);
    }

    std::move()

    return 0;
}