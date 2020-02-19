//
// Created by gjk on 2020/2/1.
//
/*
 * 快速选择算法
 * 找到数组中第k小的数，并保证在数组中该数左边都小于等于它，右边都大于等于它
 * 时间复杂度：平均O(N),最坏O(N^2)
 * 空间复杂度：O(1)
 * std::nth_element实现了这个算法
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int partition(vector<int>& a, int l, int r){
    if(l>=r)return l;
    int pivot = a[l];
    while(l<r){
        while(l<r && pivot<=a[r])r--;
        swap(a[l], a[r]);
        while(l<r && pivot>=a[l])l++;
        swap(a[l], a[r]);
    }
    a[l] = pivot;
    return l;
}

int QuickSelect(vector<int>& a, int l, int r, int k)
{
    if(l>=r)return a[l];
    int pivot = partition(a, l, r);
    if(pivot+1 == k){       //当前中点就是第k个点
        return a[pivot];
    }else if(pivot+1>k){    //pivot比k多了，则k在左边
        return QuickSelect(a, l, pivot, k);
    }else {
        return QuickSelect(a, pivot+1, r, k);
    }
}

int main()
{
    vector<int> a = {4,5,3,2,6,1,8,9,7}, b = a;
    int k = 4;
    cout<<QuickSelect(a, 0, 8, 4)<<endl;
    for(auto x:a)cout<<x<<" ";
    cout<<endl;

    nth_element(b.begin(), b.begin()+k, b.end());
    cout<<*(b.begin()+k)<<endl;
    for(auto x:b)cout<<x<<" ";
    cout<<endl;
    return 0;
}
