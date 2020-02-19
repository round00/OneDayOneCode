//
// Created by gjk on 2020/2/1.
//
/*
 * 三路parition
 * 将等于pivot的集中在中间，小于pivot的在左边，大于pivot的在右边
 * 时间复杂度：O(N)
 * 空间复杂度：O(1)
 */
#include <algorithm>
#include <iostream>
using namespace std;
void threeWayPartition(int a[], int l, int r, int pivot)
{
    if(l>=r)return;
    int i = l, j = l, k = r;    //[l,i)<pivot, [i,j)==pivot, [j,k)>pivot
    while(j<k){
        if(a[j] == pivot){
            j++;
        }else if(a[j] < pivot){
            swap(a[i++], a[j++]);   //这里i原指向第一个pivot，j指向最后一个pivot的后面的位置，所以i和j都要++
        }else{
            swap(a[j], a[--k]);
        }
    }
}

int main()
{
    int a[] = { 2,0,2,1,1,0};
    threeWayPartition(a, 0, 6, 1);
    for(auto x:a)cout<<x<<" ";
    cout<<endl;
    advance()
    return 0;
}

