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


//带统计逆序对的
int mergesort(int a[], int l, int r){
    if(l>=r)return 0;
    int mid = (l+r)/2;
    int lc = mergesort(a, l, mid);
    int rc = mergesort(a, mid+1, r);
    lc += rc;

    int t[r-l+1];
    int i = l, j = mid+1, k = 0;
    while(i<=mid && j<=r){
        if(a[i]<=a[j])t[k++] = a[i++];
        else{
            lc += mid-i+1;
            t[k++] = a[j++];
        }
    }
    while(i<=mid)t[k++] = a[i++];
    while(j<=r)t[k++] = a[j++];
    for(i = 0; i<k; ++i){
        a[i+l] = t[i];
    }
    return lc;
}
int main()
{
    int a[] = {4,5,3,2,6,1,8,9,7};
    int n = sizeof(a) / sizeof(int);

    int ni = mergesort(a, 0, n-1);
    for(auto x: a){
        printf("%d ", x);
    }
    printf("%d\n", ni);
    return 0;
}

//3+3+2+1+1+1+1