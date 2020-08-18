//
// Created by gjk on 2020/3/13.
// 工厂模式，消费者不关心他所需要的类是如何创建的时候
//
#include <iostream>
using namespace std;

class Car{
public:
    virtual void run() = 0;
    virtual ~Car() = default;
};

class Bench : public Car{
public:
    void run() override {
        cout<<"I'm Bench"<<endl;
    }
};

class BMW : public Car{
public:
    void run() override {
        cout<<"I'm BMW"<<endl;
    }
};

//=================简单工厂模式=================
enum CAR_TYPE{
    CAR_BENCH,
    CAR_BMW
};
class SimpleFactory{
public:
    Car* CreateCar(CAR_TYPE carType){
        switch (carType){
            case CAR_BENCH:
                return new Bench;
            case CAR_BMW:
                return new BMW;
            default:
                return nullptr;
        }
    }
};
//=================工厂方法模式=================
class FunctionFactory{
public:
    virtual Car* CreateCar() = 0;
    virtual ~FunctionFactory() = default;
};
class BenchFacoty : public FunctionFactory{
public:
    Car *CreateCar() override {
        return new Bench;
    }
};
class BMWFactory : public FunctionFactory{
public:
    Car *CreateCar() override {
        return new BMW;
    }
};
//=================抽象工厂模式=================
class Mask{ //口罩，现在这个时间，任何工厂都可以生产口罩
public:
    virtual void breathe() = 0;
    virtual ~Mask() = default;
};
class BenchMask : public Mask{
public:
    void breathe() override {
        cout<<"I'm BenchMask"<<endl;
    }
};

class BMWMask : public Mask{
public:
    void breathe() override {
        cout<<"I'm BMWMask"<<endl;
    }
};

class AbstractFactory{
public:
    virtual Car* CreateCar() = 0;
    virtual Mask* CreateMask() = 0;
    virtual ~AbstractFactory() = default;
};
class BenchSeriesFacoty : public AbstractFactory{
public:
    Car *CreateCar() override {
        return new Bench;
    }

    Mask *CreateMask() override {
        return new BenchMask;
    }
};
class BMWSeriesFactory : public AbstractFactory{
public:
    Car *CreateCar() override {
        return new BMW;
    }

    Mask *CreateMask() override {
        return new BMWMask;
    }
};



int main(){
//=================简单工厂模式=================
//每次增加新品种时需要修改工厂代码
    SimpleFactory simpleFactory;
    Car* car = simpleFactory.CreateCar(CAR_BENCH);
    car->run(); //I'm Bench
    delete car;

//=================工厂方法模式=================
//每次增加新品种时创建新品种的类型和工厂类型即可，不用修改工厂代码
    FunctionFactory* functionFactory = new BMWFactory;
    Car* car1 = functionFactory->CreateCar();   //I'm BMW
    car1->run();
    delete functionFactory; delete car1;

//=================抽象工厂模式=================
//这个抽象工厂模式感觉和工厂方法模式没什么本质区别
    AbstractFactory* abstractFactory = new BenchSeriesFacoty;
    Mask* mask = abstractFactory->CreateMask();
    mask->breathe();    //I'm BenchMask
    delete abstractFactory; delete mask;
    return 0;
}