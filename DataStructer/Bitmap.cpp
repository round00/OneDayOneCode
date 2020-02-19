//
// Created by gjk on 2019/12/13.
//
/*
 * Bitmap用一个数组来标记海量的数据
 * 例如一个int的每一位标识一个数是否存在，那么一个int就能标记32个数。
 * 这样标记N个数只需要N/32个int就可以了。
 * */
#include <string.h>
#include <stdio.h>

class Bitmap{
public:
    //n是最大的数字大小
    Bitmap(int N){
        one = 1;
        int maxn = N/32+1;
        vis = new unsigned int[maxn];
        for(int i = 0;i<maxn; ++i)vis[i] = 0;

    }
    ~Bitmap(){
        if(vis){
            delete [] vis;
            vis = nullptr;
        }
    }
    //0:0-31, 1:32-63, 2:64-95
    void add(int v){
        vis[v/32] |= one<<(v%32);
    }
    bool isExist(int v){
        return vis[v/32] & one<<(v%32);
    }
private:
    unsigned int *vis;
    unsigned int one;
};

int main(){
    Bitmap bm(100);
    for(int i = 0;i<=100; ++i){
        printf("i=%d  ", i);
        printf("%d ", bm.isExist(i));
        bm.add(i);
        printf("%d\n", bm.isExist(i));
    }
    return 0;
}