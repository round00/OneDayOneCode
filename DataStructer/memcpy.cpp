//
// Created by gjk on 2020/3/9.
//
#include <stdio.h>
void *my_memcpy(void *dst, const void *src, int n)
{
    if(!dst || !src)return nullptr;
    char *pdst = (char*)dst;
    char *psrc = (char*)src;
    if(pdst >psrc && psrc+n > pdst){  //override
        pdst += n-1;
        psrc += n-1;
        while(n--)*pdst-- = *psrc--;
    }else{
        while(n--)*pdst++ = *psrc++;
    }
    return dst;
}
int main(){
    char buf[12] = "nihao";
    int len = 5;
    char *p = buf + 2;
    my_memcpy(p, buf, len+1);
    printf("%s\n",p);
    return 0;
}