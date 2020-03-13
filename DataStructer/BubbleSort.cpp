//
// Created by gjk on 2020/3/13.
//
#include <stdio.h>
void BubbleSort(int a[], int n){
    for(int i = 0; i<n; ++i){
        for(int j = 0;j<n-i-1; ++j){
            if(a[j]>a[j+1]){
                int t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
            }
        }
    }
}

int main(){
    int a[] = {3,5,2,1,6,7,4};
    BubbleSort(a, 7);
    for(int i = 0;i<7; ++i){
        printf("%d ", a[i]);
    }
    printf("\n");
    
    return 0;
}