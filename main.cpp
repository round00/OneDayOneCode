#include <stdio.h>
#include <iostream>
#include <deque>
#include <string>
#include <vector>
using namespace std;

int fun(int a,int b,int c,int d,int e,int f,int g, int h){
    printf("a = 0x%p\n", &a);
    printf("b = 0x%p\n", &b);
    printf("c = 0x%p\n", &c);
    printf("d = 0x%p\n", &d);
    printf("e = 0x%p\n", &e);
    printf("f = 0x%p\n", &f);
    printf("g = 0x%p\n", &g);
    printf("h = 0x%p\n", &h);
    return 0;
}

int main(int argc, char *argv[])
{
    string s = "\0";
    if(s.empty())printf("nihao");
    return 0;
}
