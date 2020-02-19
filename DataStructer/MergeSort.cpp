//
// Created by gjk on 2020/1/31.
//
#include <stdio.h>

void merge(int a[], int l, int mid, int r){
    int t[r-l+1], it = 0;
    int i = l, j = mid+1;
    while(i<=mid && j<=r)t[it++] = a[i]<=a[j]?a[i++]:a[j++];
    while(i<=mid)t[it++] = a[i++];
    while(j<=r)t[it++] = a[j++];

    for(i = 0;i<it;++i,++l)a[l] = t[i];
}

void MergeSort(int a[], int l, int r)
{
    if(l>=r)return;
    int mid = (l+r)/2;
    MergeSort(a, l, mid);
    MergeSort(a, mid+1, r);
    merge(a, l, mid, r);
}

int main()
{
    int a[] = {4,5,3,2,6,1,8,9,7};
    int n = sizeof(a) / sizeof(int);

    MergeSort(a, 0, n-1);
    for(auto x: a){
        printf("%d ", x);
    }
    printf("\n");
    return 0;
}