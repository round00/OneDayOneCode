//
// Created by gjk on 2019/12/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>    //swap
//这是大顶堆

//从index开始往上调整，如果父节点比当前节点小，那就交换。直到根节点
void shiftUp(int heap[], int index){
    while(index > 0){
        int fa = (index-1)/2;
        if(heap[fa]<heap[index]){
            std::swap(heap[fa], heap[index]);
        }
        index = fa;
    }
}
//从index开始往下调整，从两个子节点中选出一个最大的和当前节点比，
// 如果比当前节点大，那就交换。直到index>=n即超出堆的大小。
void shiftDown(int heap[], int n, int index){
    while(index < n){
        int max = heap[index], maxindex = index;
        if(index*2+1 < n && max < heap[index*2+1]){
            max = heap[index*2+1];
            maxindex = index*2+1;
        }
        if(index*2+2 < n && max < heap[index*2+2]){
            max = heap[index*2+2];
            maxindex = index*2+2;
        }

        if(maxindex == index)break;
        std::swap(heap[index], heap[maxindex]);
        index = maxindex;
    }
}
//将v插入到heap中
void insert(int heap[], int n, int v){
    heap[n] = v;
    shiftUp(heap, n);
}
//从heap中返回堆顶，并且从堆中删除它。将堆的大小减一。
int pop(int heap[], int* n){
    int res = heap[0];
    heap[0] = heap[--(*n)];
    shiftDown(heap, (*n), 0);
    return res;
}
//从数组a构建堆，存放到heap中
void buildHeap(int heap[], int a[], int n){
    for(int i = 0;i<n; ++i){
        insert(heap, i, a[i]);
    }
}
//堆排序，每次从堆中取出堆顶。时间复杂度O(nlogn)
// 需要借助O(n)辅助数组来存放堆。
void HeapSort(int a[], int n){
    int *heap = (int*)malloc(n* sizeof(int));
    buildHeap(heap, a, n);
    int heapsz = n;
    for(int i = 0;i<n; ++i){
        a[i] = pop(heap, &heapsz);
    }
    free(heap);
}
int main(){
    int a[] = {4,5,3,2,1, 6,7};
    HeapSort(a, 7);
    for(int i = 0;i<7; ++i){
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}