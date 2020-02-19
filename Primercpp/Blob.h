//
// Created by gjk on 2020/1/25.
//

#ifndef ONEDAYONECODE_BLOB_H
#define ONEDAYONECODE_BLOB_H

#include <vector>
#include <memory>
#include <string>


//=================================
template <typename T> class BlobPtr;
template <typename T> class Blob;
template <typename T>
bool operator==(const Blob<T>& a, const Blob<T>& b){
    return true;
}

//=================================
template <typename T>
class Blob
{
public:

    friend class BlobPtr<T>;
    friend bool operator==<T>(const Blob<T>&, const Blob<T>&);
    friend T;

    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    Blob();
    Blob(std::initializer_list<T> il):data(std::make_shared<std::vector<T>>(il)){}
    template <typename It>
    Blob(It b, It e);

    size_type size()const {return data->size();}
    bool empty()const{return data->empty();}

    void push_back(const T& t){data->push_back(t);}
    void push_back(T &&t){data->push_back(std::move(t));}

    void pop_back(){
        check(0, "pop_back on empty Blob");
        data->pop_back();
    }

    T& back(){
        check(0, "back on empty Blob");
        return data->back();
    }
    T& operator[](size_type i);

private:
    std::shared_ptr<std::vector<T>> data;
    void check(size_type i, const std::string& msg)const;
};

typedef Blob<std::string> StrBlob;
typedef Blob<int> IntBlob;

template <typename T> using twin = std::pair<T, T>;
template <typename T> using partNo = std::pair<T, unsigned int>;
twin<std::string> names;
twin<int> win_loss;

//类外定义成员函数
template <typename T>
Blob<T>::Blob()
    :data(std::make_shared<std::vector<std::string>>())
{
}

template <typename T>
void Blob<T>::check(size_type i, const std::string& msg)const{
    if(i>=data->size()){
        throw std::out_of_range(msg);
    }
}

template <typename T>
T& Blob<T>::operator[](size_type i){
    check(i, "subscript out of range");
//    return data->operator[](i);
    return (*data)[i];
}

template <typename T>
template <typename It>
Blob<T>::Blob(It b, It e)
    :data(std::make_shared<std::vector<T>>(b, e))
{
}


//==========================================

template <typename T>
class BlobPtr
{
public:
    BlobPtr():curr(0){}
    BlobPtr(Blob<T>& a, size_t sz = 0):
        wptr(a.data), curr(sz){}
    T& operator*()const{
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }

    //下面这两种形式都是对的，在模板类作用域内有没有T都是可以的
    //前置
    BlobPtr& operator++();
    BlobPtr<T>& operator--();
    //后置
    BlobPtr operator++(int);
    BlobPtr operator--(int);

private:
    std::shared_ptr<std::vector<T>> check(size_t, const std::string& msg)const;

    std::weak_ptr<std::vector<T>> wptr;
    std::size_t curr;
};

template <typename T>
BlobPtr<T>& BlobPtr<T>::operator++() {
    curr++;
    return check(curr, "operator++ out of range");
}
template <typename T>
BlobPtr<T>& BlobPtr<T>::operator--() {
    curr--;
    return check(curr, "operator out of range");
}

template <typename T>
BlobPtr<T> BlobPtr<T>::operator++(int) {
    BlobPtr ret = *this;
    ++*this;
    return ret;
}

template <typename T>
BlobPtr<T> BlobPtr<T>::operator--(int) {
    BlobPtr ret = *this;
    --*this;
    return ret;
}



#endif //ONEDAYONECODE_BLOB_H
