//
// Created by gjk on 2019/12/17.
//

//  这个BST写的不好，没有删除操作，可以参考AVL树的写法--AVLTree.cpp

#include <stdio.h>
struct Node{
    int val;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    Node(int v = 0){
        val = v;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
    }
};
typedef struct BinarySearchTree{
    struct Node* root;
    BinarySearchTree(){
        root = nullptr;
    }
    //插入一个节点，成功返回true，失败（BST中已经有该值的节点存在）返回false
    bool insert(int val){
        if(root == nullptr){
            root = new Node(val);
            return true;
        }

        Node* it = root;
        while(it != nullptr){
            if(it->val == val)return false;
            else if(it->val > val){
                if(it->left==nullptr){
                    it->left = new Node(val);
                    break;
                }
                it = it->left;
            }
            else {
                if(it->right== nullptr){
                    it->right = new Node(val);
                    break;
                }
                it = it->right;
            }
        }
        return true;
    }
    //true：找到， false：没找到
    bool search(int val){
        Node* it = searchNode(val);
        if(it == nullptr)return false;
        return true;
    }
    //BST中的最小值
    int minValue(){
        Node* node = getMinNode(root);
        if(node == nullptr)return 0;
        return node->val;
    }
    //BST中的最大值
    int maxValue(){
        Node* node = getMaxNode(root);
        if(node == nullptr)return 0;
        return node->val;
    }
    //中序遍历，即从小到大排序
    void print(){
        midTraverse(root);
        printf("\n");
    }

private:
    Node* searchNode(int val){
        Node* it = root;
        while(it != nullptr){
            if(it->val==val)return it;
            else if(it->val > val)it = it->left;
            else it = it->right;
        }
        return nullptr;
    }
    Node* getMinNode(Node* rt){
        if(rt == nullptr)return nullptr;
        Node* it = rt;
        while(it->left != nullptr)it = it->left;
        return it;
    }
    Node* getMaxNode(Node *rt){
        if(rt == nullptr)return nullptr;
        Node* it = rt;
        while(it->right != nullptr)it = it->right;
        return it;
    }
    //查找 大于node节点的下一节点: 其实就是中序遍历的下一个节点
    Node* nextLargerNode(Node* node){
        if(node== nullptr)return nullptr;
        //如果有右子树，那么下一个节点一定是右子树中的最小节点
        if(node->right != nullptr){
            return getMinNode(node->right);
        }
        //没有右子树，中序遍历只能往回走了，一直走到某个节点，该节点是其父节点的左子节点
        // 则：该节点的父节点就是下一个节点
        while(node != node->parent->left){
            node = node->parent;
            if(node == nullptr){
                return nullptr;
            }
        }
        return node->parent;
    }
    void midTraverse(Node*rt){
        if(rt==nullptr)return;
        midTraverse(rt->left);
        printf("%d ", rt->val);
        midTraverse(rt->right);
    }
}BSTree;
int main(){
    BSTree bst;
    int a[] = {4,3,5,6,1,2,9,8};
    for(auto x:a){
        bst.insert(x);
    }
    bst.print();
    printf("min=%d\n", bst.minValue());
    printf("max=%d\n", bst.maxValue());
    int b[] = {1,2,5,7,10,11};
    for(auto x:b){
        printf("search %d=%d\n", x, bst.search(x));
    }


    return 0;
}