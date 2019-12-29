//
// Created by gjk on 2019/12/29.
//

#ifndef ONEDAYONECODE_MYSHAREDPTR_H
#define ONEDAYONECODE_MYSHAREDPTR_H
template <typename T>
class CMySharedPtr{
public:
    explicit CMySharedPtr(T* ptr)
        :m_pRawPtr(ptr)
    {
        m_refCount = new int(1);
    }
    ~CMySharedPtr(){
        (*m_refCount) --;
        if(*m_refCount == 0){
            delete m_pRawPtr;
            delete m_refCount;
        }
    }

    //copying behaviors
    CMySharedPtr(const CMySharedPtr& rhs){
        m_pRawPtr = rhs.m_pRawPtr;
        m_refCount = rhs.m_refCount;
        (*m_refCount) ++;
    }
    CMySharedPtr& operator=(const CMySharedPtr& rhs){
        if(this == &rhs)return *this;
        m_pRawPtr = rhs.m_pRawPtr;
        m_refCount = rhs.m_refCount;
        (*m_refCount) ++;
    }

    T* get()const {
        return m_pRawPtr;
    }

    int getRefCount()const{
        return *m_refCount;
    }

    T* operator->(){
        return m_pRawPtr;
    }
    T& operator*(){
        return *m_pRawPtr;
    }

private:
    T* m_pRawPtr;
    int* m_refCount;
};
#endif //ONEDAYONECODE_MYSHAREDPTR_H
