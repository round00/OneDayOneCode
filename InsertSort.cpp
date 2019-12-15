//
// Created by gjk on 2019/12/15.
//
#include <stdio.h>
void InsertSort(int a[], int l, int r){
    for(int i = l+1; i<=r; ++i){
        int t = a[i], j;
        for(j = i-1; j>=l && a[j]>t; --j){
            a[j+1] = a[j];
        }
        a[j+1] = t;
    }
}

int main(){
    int a[] = {4,3,5,6,1,8,7,9};
    InsertSort(a, 0, 7);
    for(auto i:a){
        printf("%d ", i);
    }
    printf("\n");
    return 0;
}