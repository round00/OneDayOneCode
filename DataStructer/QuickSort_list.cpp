//
// Created by gjk on 2019/12/15.
//

#include <stdio.h>
#include <algorithm>
struct Node{
    int val;
    struct Node* next;
    Node(int v = 0){
        val = v;
        next = nullptr;
    }
};
void QuickSort_list(Node* begin, Node* end){
    if(begin == nullptr || begin == end)
        return;

    Node* p1 = begin;       //左子链表的尾节点，这里尾节点的意思是最后一个节点的下一个节点，类似vector.end()。又是最后pivot的位置
    Node* p2 = begin->next; //p2用来不断扩大右子链表的大小
//    int pivot = begin->val; //begin is just the pivot
    while(p2 != end){
        if(p2->val < begin->val){
            p1 = p1->next;
            std::swap(p1->val, p2->val);
        }
        p2 = p2->next;
    }

    std::swap(begin->val, p1->val);
    QuickSort_list(begin, p1);
    QuickSort_list(p1->next, end);
}

Node* create_list(int a[], int n){
    Node*head = new Node(), *it = head;
    for(int i = 0;i<n; ++i){
        Node* t = new Node(a[i]);
        it->next = t;
        it = it->next;
    }
    Node*ret = head->next;
    delete head;
    return ret;
}
void print_list(Node* head){
    if(!head){
        printf("\n");
        return;;
    }
    printf("%d ", head->val);
    print_list(head->next);
}
int main(){
    int a[] = {4,3,5,6,1,8,7,9};
    Node* head = create_list(a, 7);
    print_list(head);
    QuickSort_list(head, nullptr);
    print_list(head);
    return 0;
}