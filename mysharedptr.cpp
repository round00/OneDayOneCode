//
// Created by gjk on 2019/12/29.
//

#include "mysharedptr.h"
#include <stdio.h>
class A{
public:
    A(){val = 0;}
    void print(){
        printf("Hello World,val=%d\n",val++);
    }

private:
    int val;
};

int main(){
    CMySharedPtr<A> pA(new A());
    pA->print();

    printf("1.ref count=%d\n", pA.getRefCount());
    {
        CMySharedPtr<A> pA1(pA);
        (*pA1).print();
        printf("2.ref count=%d\n", pA.getRefCount());
        {
            CMySharedPtr<A> pA2 = pA1;
            pA2->print();
            printf("3.ref count=%d\n", pA.getRefCount());
        }
        printf("4.ref count=%d\n", pA.getRefCount());
    }
    printf("5.ref count=%d\n", pA.getRefCount());





    return 0;
}