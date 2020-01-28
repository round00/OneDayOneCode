//
// Created by gjk on 2020/1/28.
//
#include <iostream>
//AVL树，平衡二叉搜索树，Self-balancing binary search tree：
//是一颗空树，或者左右两个子树的高度差不超过1，并且左右两个子树也是AVL树


template <typename E>
class AVLNode
{
public:
    explicit AVLNode(E val_):
        val(val_),
        left(nullptr), right(nullptr), parent(nullptr),
        balance(0)
    {}

    E           val;
    AVLNode*    left;
    AVLNode*    right;
    AVLNode*    parent;
    int         balance;
};

template <typename E>
class AVLTree
{
public:
    AVLTree():root(nullptr){}
    AVLNode<E>*     search(E v);
    void            insert(E v);
    AVLNode<E>*     getMinNode();
    AVLNode<E>*     getNextNode(AVLNode<E>* node);

    void            del(E v);

    void            print();
    void            mid(AVLNode<E> *);
private:
    AVLNode<E>*     root;

    AVLNode<E>*     getMinNode(AVLNode<E>* rt);

    void            delWithLess2Child(AVLNode<E>* node);
    void            delWith2Child(AVLNode<E>* node);

    AVLNode<E>*     Rrotate(AVLNode<E>* node);
    AVLNode<E>*     Lrotate(AVLNode<E>* node);
    AVLNode<E>*     LRrotate(AVLNode<E>* node);
    AVLNode<E>*     RLrotate(AVLNode<E>* node);

    void            setBalance(AVLNode<E>* node);
    int             height(AVLNode<E>*  node);
    void            reBalance(AVLNode<E>* node);

};

/*
 * 查找值等于v的节点，并返回该节点。
 * 如果树为空则返回空
 * 如果树种不存在该节点，则返回该节点应该插入位置的父节点
 */
template <typename E>
AVLNode<E>* AVLTree<E>::search(E v) {
    auto it = root;
    while(it){
        if(it->val == v)return it;      //找到直接返回
        else if(it->val > v){
            if(it->left)it = it->left;  //有左子树，去左子树找
            else return it;             //没有左子树了，直接返回当前点
        }
        else {
            if(it->right)it = it->right;
            else return it;
        }
    }
    return nullptr;
}

/*
 * 将v新建一个节点并插入到AVL树中
 */
template <typename E>
void AVLTree<E>::insert(E v) {
    auto father = search(v);
    if(!father){        //说明树是空的，还没有节点
        root = new AVLNode<E>(v);
    }else {
        if(father->val==v)return;
        auto newNode = new AVLNode<E>(v);
        newNode->parent = father;
        if(father->val>v){  //插入为左子树
            father->left = newNode;
            reBalance(father);
        }else{              //插入为右子树
            father->right = newNode;
            reBalance(father);
        }
    }
}

/*
 * 找个树中的最小节点
 */
template <typename E>
AVLNode<E>* AVLTree<E>::getMinNode() {
    return getMinNode(root);
}

template <typename E>
AVLNode<E>* AVLTree<E>::getMinNode(AVLNode<E>* rt) {
    if(!rt)
        return nullptr;
    auto it = rt;
    while(it->left)it = it->left;
    return it;
}

/*
 * 找到node的后继节点
 */
template <typename E>
AVLNode<E>* AVLTree<E>::getNextNode(AVLNode<E> *node) {
    if(!node)return nullptr;
    //如果有右子树，那么后继节点一定是右子树中的最小节点
    if(node->right != nullptr){
        return getMinNode(node->right);
    }
    //没有右子树，中序遍历只能往回走了，一直走到某个节点，该节点是其父节点的左子节点
    // 则：该节点的父节点就是后继节点
    while(node != node->parent->left){
        node = node->parent;
        if(node == nullptr){
            return nullptr;
        }
    }
    return node->parent;
}
/*
 * 删除值为v的节点
 */
template <typename E>
void AVLTree<E>::del(E v) {
    auto node = search(v);
    if(node && node->val==v){
        if(node->left && node->right){
            delWith2Child(node);
        }else{
            delWithLess2Child(node);
        }
    }
}

/*
 * 删除节点node，请保证node有两个子节点
 */
template <typename E>
void AVLTree<E>::delWith2Child(AVLNode<E>* node) {
    if(!node)return;
    if(!node->left || !node->right)return;      //保证node有两个子节点
    //有两个子节点的情况下，先找到当前节点的后继节点，
    // 因为有右子树，并且后继节点就是右子树的最小节点，
    // 所以把后继节点和当前节点的值进行交换，然后删除后继节点就好了
    auto nextNode = getNextNode(node);
    node->val = nextNode->val;
    delWithLess2Child(nextNode);
}

/*
 * 删除node，请保证node有少于两个子节点
 */
template <typename E>
void AVLTree<E>::delWithLess2Child(AVLNode<E> *node) {
    if(!node)return;
    if(node->left && node->right)return;    //这里只处理少于两个节点的情况

    if(!node->parent){      //没有父节点，那node就只能是根节点了
        if(node->left){
            root = node->left;
            root->parent = nullptr;
        }else if(node->right){
            root = node->right;
            root->parent = nullptr;
        }else root = nullptr;

    }else{
        if(node == node->parent->left){ //node是父节点的左子树
            if(node->left){
                node->left->parent = node->parent;
                node->parent->left = node->left;
            }else if(node->right){
                node->right->parent = node->parent;
                node->parent->left = node->right;
            }else node->parent->left = nullptr;
        }else {                         //node是父节点的右子树
            if(node->left){
                node->left->parent = node->parent;
                node->parent->right = node->left;
            }else if(node->right){
                node->right->parent = node->parent;
                node->parent->right = node->right;
            }else node->parent->right = nullptr;
        }
    }
    reBalance(node->parent);
    delete node;
}

/*
 * 右旋操作
 */
template <typename E>
AVLNode<E>* AVLTree<E>::Rrotate(AVLNode<E> *node) {
    if(!node)return nullptr;

    auto pivot = node->left;
    if(node->parent){
        if(node == node->parent->left){
            node->parent->left = pivot;
        }else {
            node->parent->right = pivot;
        }
    }


    pivot->parent = node->parent;
    node->left = pivot->right;
    pivot->right = node;
    node->parent = pivot;
    if(node->left)node->left->parent = node;

    setBalance(node);
    setBalance(pivot);
    return pivot;
}

template <typename E>
AVLNode<E>* AVLTree<E>::Lrotate(AVLNode<E> *node) {
    if(!node)return nullptr;

    auto pivot = node->right;
    if(node->parent){
        if(node == node->parent->left){
            node->parent->left = pivot;
        }else {
            node->parent->right = pivot;
        }
    }

    pivot->parent = node->parent;
    node->right = pivot->left;
    pivot->left = node;
    node->parent = pivot;
    if(node->right)node->right->parent = node;

    setBalance(node);
    setBalance(pivot);
    return pivot;
}

/*
 * 先左旋node的左子树，再右旋node
 */
template <typename E>
AVLNode<E>* AVLTree<E>::LRrotate(AVLNode<E> *node) {
    Lrotate(node->left);
    return Rrotate(node);
}

/*
 * 先右旋node的右子树，再左旋node
 */
template <typename E>
AVLNode<E>* AVLTree<E>::RLrotate(AVLNode<E> *node) {
    Rrotate(node->right);
    return Lrotate(node);
}

template <typename E>
void AVLTree<E>::setBalance(AVLNode<E> *node) {
    if(node){
        node->balance = height(node->left)-height(node->right);
    }
}

template <typename E>
int AVLTree<E>::height(AVLNode<E> *node) {
    if(!node)return 0;
    int l = height(node->left), r = height(node->right);
    return 1 + (l>r?l:r);
}

/*
 * 重新调整树的平衡,从当前节点一直调整到根节点
 */
template <typename E>
void AVLTree<E>::reBalance(AVLNode<E> *node) {
    if(!node)return;

    setBalance(node);
    if(node->balance==2){      //左子树高了
        if(node->left->balance>=0){
            node = Rrotate(node);
        }else{
            node = LRrotate(node);
        }
    }else if(node->balance==-2){    //右子树高了
        if(node->right->balance<=0){
            node = Lrotate(node);
        }else{
            node = RLrotate(node);
        }
    }

    if(node->parent)reBalance(node->parent);
    else root = node;
}

template <typename E>
void AVLTree<E>::print() {
    std::cout<<"======================="<<std::endl;
    mid(root);
    std::cout<<"======================="<<std::endl;
}

//中序遍历
template <typename E>
void AVLTree<E>::mid(AVLNode<E> *rt) {
    if(!rt)return;
    mid(rt->left);
    std::cout<<"val="<<rt->val << " balance="<<rt->balance<<std::endl;
    mid(rt->right);
}
int main(){
    int a[] = {5,6,7,3,4,4,5,5,3,8,9,10,11,12};
    AVLTree<int> avl;
    for(auto x:a){
        avl.insert(x);
    }
    avl.print();
    int b[] = {6,11,12,3,3,9};
    for(auto x:b)avl.del(x);
    avl.print();



    return 0;
}