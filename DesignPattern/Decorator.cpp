//
// Created by gjk on 2020/3/15.
// 装饰器模式，可以为已有类添加功能
//

#include <iostream>
using namespace std;

class Car{
public:
    ~Car(){}
    void run(){
        cout<<"I'm Running"<<endl;
    }
};

class CarDecorator{
public:
    CarDecorator(Car* car):m_car(car){}
    void run() {
        cout<<"Open the car door"<<endl;
        m_car->run();
    }

private:
    Car*    m_car;
};

int main(){
    
    Car* car = new Car();
    CarDecorator carDecorator(car);
    carDecorator.run();
    
    return 0;
}