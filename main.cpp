#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class A{
    A(){}
    void fun(int x);
public:

};
class B:public A{
public:

};
extern const int a = 1;
int main()
{
    int x = 65535+81;
    uint16_t y = x;
    cout<<y<<endl;
    return 0;
}