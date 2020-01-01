//
// Created by gjk on 2019/12/29.
//

#ifndef ONEDAYONECODE_QUOTE_H
#define ONEDAYONECODE_QUOTE_H

#include <string>

class Quote{
public:
    Quote() = default;
    Quote(const std::string& book, double sale_price)
        :price(sale_price),bookNo(book){}
    virtual ~Quote()= default;

    std::string isbn()const{return bookNo;}
    virtual double net_price(std::size_t n)const{return n*price;}

protected:
    double price = 0.0; //类内初值，有的编译器可能不支持这样
private:
    std::string bookNo;
};

class Bulk_Quote:public Quote{
public:
    Bulk_Quote(const std::string& book, double sale_price, std::size_t qty, double disc)
        :min_qty(qty), discount(disc), Quote(book, sale_price)
        {}

    //override C++11标准的关键字
    double net_price(std::size_t n)const override;

private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};

#endif //ONEDAYONECODE_QUOTE_H
