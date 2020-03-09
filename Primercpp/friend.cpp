//
// Created by gjk on 2020/3/4.
// 类为了让其他类或者函数访问它的非公有成员，可以将他们声明为类的友元
// 友元只能声明在类内部，但是类内部的位置不限。但是最好集中在一块声明
//

#include <stdio.h>
class User;

class Pet{
    friend class User;
public:
    Pet(){}
private:

    int type;
};

class User{
public:
    User(int _id, int _age):id(_id), age(_age){}

    void setPet(Pet _pet){
        pet.type = _pet.type;
    }

private:
    int id;
    int age;
    Pet pet;

    friend void print(const User&);
    friend void addAge(User& user){
        user.age++;
    }

};

void print(const User& user){
    printf("%d %d\n", user.id, user.age);
}
//这个声明是必须的
void addAge(User& user);
int main(){
    User user(2,3);
    addAge(user);
    print(user);
    return 0;
}