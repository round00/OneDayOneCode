//
// Created by gjk on 2019/12/18.
//
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <functional>
using namespace std;

class Person{
public:
    Person(){}
    Person(string name,int age):name_(name), age_(age){}

    void setName(string name){name_ = name;}
    string getName()const{return name_;}
    void setAge(int age){age_ = age;}
    int getAge()const{return age_;}


    //输入输出运算符必须是非成员函数，一般以类外部的普通函数来重载，或者友元函数
    //如果在上述两种定义方式都使用了，会出现redefinition错误
    friend ostream& operator<<(ostream& os, const Person& person){
        os<<person.name_<<" is "<<person.age_<<" years old! "; //尽量不带换行
        return os;
    }
    friend istream&operator>>(istream& is, Person& person){
        is>>person.name_>>person.age_;
        //输入运算符必须处理出错的情况
        if(!is)person = Person("Error", -1);
        return is;
    }

    //赋值运算符，无论形参是什么，都必须定义为类的成员函数
    Person& operator=(const Person& rhs){
        if(this == &rhs)return *this;
        name_ = rhs.name_;
        age_ = rhs.age_;
        return *this;
    }
    Person& operator=(initializer_list<int> il){
        if(il.begin()==il.end())return *this;
        name_ = to_string(*il.begin());
        age_ = *(il.begin()+1);
        return *this;
    }

    //复合赋值运算符不必非得定义为成员函数，但推荐做法是定义为成员函数。
    Person& operator+=(const Person& rhs){
        name_ = name_+" and "+rhs.name_;
        age_ += rhs.age_;
        return *this;
    }

    //下标运算符一定要定义为成员函数, 通常定义const和非const两个版本
    string& operator[](std::size_t index){
        if(index >= sons.size())return name_;
        return sons[index];
    }
    const string& operator[](std::size_t index)const{
        if(index >= sons.size())return name_;
        return sons[index];
    }
    //递增和递减运算符：推荐定义为成员函数
    //需要实现前置版本和后置版本
    //与内置版本保持一致，前置版本递增和递减运算符需要返回递增和递减后的引用
    Person& operator++(){
        age_++;
        return *this;
    }
    Person& operator--(){
        age_--;
        return *this;
    }
    //为了与前置版本区分开，后置版本的定义方法是：
    // 增加一个int形参，但不使用，编译器会自动传一个0给它
    // 与内置版本保持一致，后置递增和递减运算符需要返回递增和递减前的值
    // 其调用方式其实是obj.operator++(0)
    Person operator++(int nouse){
        Person ans = *this;
        ++*this;
        return ans;
    }
    Person operator--(int nouse){
        Person ans = *this;
        --*this;
        return ans;
    }

    //函数调用运算符必须是成员函数
    int operator()(int addage){
        return age_+addage;
    }

    //类型转换运算符
    //1、必须是类内成员函数 2、不能有返回值 3、不能有参数 4、要定义为const
    explicit operator int()const{
        return age_;
    }

    //算数运算符定义为成员或非成员都可
    Person operator+(const Person& rhs){
        Person t = *this;
        t += rhs;
        return t;
    }
    bool operator==(const Person& rhs){
        return name_==rhs.name_ && age_==rhs.age_;
    }

    bool operator<(const Person& rhs){
        if(age_==rhs.age_)return name_<rhs.name_;
        return age_<rhs.age_;
    }
private:
    string name_;
    int age_;
    vector<string> sons;
};


class PersonPtr{
public:
    explicit PersonPtr(Person* p):m_ptr(p){}
    //解引用运算符,一般是指针类才重载的
    //通常定义为类的成员
    Person& operator*()const{
        return *m_ptr;
    }
    //箭头运算符必须定义为成员函数
    //箭头运算符必须要返回某个类的指针，或者是自定义了箭头运算符的其他对象
    Person* operator->()const {
        return &this->operator*();
    }
private:
    Person* m_ptr;
};


//ostream& operator<<(ostream& os, const Person& person){
//    os<<person.getName()<<" is "<<person.getAge()<<" years old. ";    //尽量不带换行
//    return os;
//}
//Person& operator+=(Person& lhs, const Person& rhs){
//    lhs.setName(lhs.getName() + " and "+rhs.getName());
//    lhs.setAge(lhs.getAge()+rhs.getAge());
//    return lhs;
//}
Person operator+(const Person& lhs, const Person& rhs){
    //如果定义了对应的复合运算符例如+=，尽量用复合运算发操作
    Person person = lhs;
    person += rhs;
    return person;
}

bool operator==(const Person& lhs, const Person& rhs){
    return lhs.getAge()==rhs.getAge() && lhs.getName()==rhs.getName();
}

bool operator!=(const Person& lhs, const Person& rhs){
    return !(lhs==rhs);
}

bool operator<(const Person& lhs, const Person& rhs){
    if(lhs.getAge() == rhs.getAge())
        return lhs.getName() < rhs.getName();
    return lhs.getAge()<lhs.getAge();
}

int add(int a,int b){
    return a+b;
}
struct divide{
    int operator()(int a,int b){
        return a/b;
    }
};
int main(){
    //函数对象模板
    plus<int> intAdd;
    negate<int> intNegate;
//    cout<<intAdd(1,2)<<endl;
//    cout<<intNegate(-1)<<endl;


    minus<int> intMinus;
    auto mod = [](int a,int b){return a%b;};
    map<string, function<int(int,int)>> binops = {
            {"+", add},
            {"-", intMinus},
            {"*", [](int a,int b){return a*b;}},
            {"/", divide()},
            {"%", mod},
    };
    cout<<binops["+"](10, 5)<<endl;
    cout<<binops["-"](10, 5)<<endl;
    cout<<binops["*"](10, 5)<<endl;
    cout<<binops["/"](10, 5)<<endl;
    cout<<binops["%"](10, 5)<<endl;


    Person person;
    cin>>person;
    cout<<person<<endl;
    Person person1("gjk", 27);
    person1.operator++(0);
    PersonPtr ptr(&person1);
    ptr->getAge();
    (*ptr).getName();

    cout<<person1+person<<endl;
    cout<<person1(1)<<endl;

    //显示的强制类型转换
    int aaa = (int)person1;
    int bbb = static_cast<int>(person1);
    cout<<aaa<<endl;
    cout<<bbb<<endl;

    return 0;
}