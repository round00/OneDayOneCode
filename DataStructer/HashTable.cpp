//
// Created by gjk on 2020/2/3.
//
#include <string>
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>
using namespace std;


template <typename T>
struct Node{
    struct Node* next;
    string key;
    T val;
    explicit Node(string k, const T& t):
        key(std::move(k)), val(t), next(nullptr)
    {}
};

template <typename T>
class HashTable{
public:
    typedef function<size_t(const string&)> HashFunc;
    explicit HashTable(int n):
        table_size(n), index(vector<Node<T>*>(n, nullptr))
    {
        hash = std::hash<string>();
    }
    ~HashTable(){
        for(auto it:index){
            while(it){
                auto t = it->next;
                delete it;
                it = t;
            }
        }
    }
    void add(const string& k, const T& v){
        auto it = getNode(k);
        if(it){
            it->val = v;
        }else {
            int h = hashCode(k);
            auto newNode = new Node<T>(k, v);
            if(index[h]){  //头插
                newNode->next = index[h];
            }
            index[h] = newNode;
        }
    }

    T* get(const string& k){
        auto it = getNode(k);
        if(it)return &(it->val);
        return nullptr;
    }


private:

    Node<T>* getNode(const string& k){
        int h = hashCode(k);
        auto it = index[h];
        while(it){
            if(it->key == k)return it;
            else it = it->next;
        }
        return nullptr;
    }

    int hashCode(const string& s){
        int h = hash(s);
        if(h<0)h = -h;
        return h%table_size;
    }

    int                 table_size;
    vector<Node<T>*>    index;
    HashFunc            hash;
};

int main()
{
    HashTable<int> ht(5);
    ht.add("one", 1);
    ht.add("two", 2);
    ht.add("three", 3);
    ht.add("four", 4);
    ht.add("five", 5);
    ht.add("six", 6);
    ht.add("seven", 7);
    ht.add("eight", 8);
    ht.add("ten", 10);

    cout<<*(ht.get("six"))<<endl;
    cout<<*(ht.get("dsa"))<<endl;

    return 0;
}