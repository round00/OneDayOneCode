//
// Created by gjk on 2019/12/14.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <stack>
//最普通的，直接选择左端点值
int selectPivot(int a[], int l, int r){
    return a[l];
}
//优化1：随机选择pivot
int selectPivotRandom(int a[], int l, int r){
    srand(time(0));
    int p = rand()%(r-l)+l;
    std::swap(a[l], a[p]);
    return a[l];
}
//优化2：三数取中选择pivot
int selectPivotThreeOfmid(int a[], int l, int r){
    int mid = (l+r)/2;
    if(a[mid]>a[r]){
        std::swap(a[mid], a[l]);
    }
    if(a[l]>a[r]){
        std::swap(a[l], a[r]);
    }
    if(a[l]<a[mid]){
        std::swap(a[l], a[mid]);
    }
    return a[l];
}

//优化3：数组长度小于10的时候改用插入排序
extern void InsertSort(int a[], int l, int r);
//优化4：选出pivot后，将数组中与pivot相同的值聚集到pivot的周围
//实现略。

//优化5：尾递归优化

int partition(int a[], int l, int r){
    if(l==r){
        return l;
    }

    int pivot = selectPivotThreeOfmid(a, l, r);
    while(l<r){
        while(l<r && a[r]>=pivot)r--;
        a[l] = a[r];
        while(l<r && a[l]<=pivot)l++;
        a[r] = a[l];
    }
    a[l] = pivot;
    return l;
}

void QuickSort(int a[], int l, int r){
    if(l>=r)return;
    //===================================
    //尾递归优化
    while(l<r){
        int mid = partition(a, l, r);
        QuickSort(a, l, mid-1);
        l = mid+1;
    }

    //===================================
    //普通递归
//    int mid = partition(a, l, r);
//    QuickSort(a, l, mid-1);
//    QuickSort(a, mid+1, r);
}
//快排非递归写法
void QuickSort_NonRecur(int a[], int l, int r){
    std::stack<int> st;
    st.push(l);
    st.push(r);
    while(!st.empty()){
        r = st.top();
        st.pop();
        l = st.top();
        st.pop();

        int mid = partition(a, l, r);
        if(l<mid-1){
            st.push(l);
            st.push(mid-1);
        }
        if(r>mid+1){
            st.push(mid+1);
            st.push(r);
        }
    }
};

int main(){
    int a[] = {4,3,5,6,1,8,7,9};
    QuickSort_NonRecur(a, 0, 7);
    for(auto i:a){
        printf("%d ", i);
    }
    printf("\n");
    return 0;
}