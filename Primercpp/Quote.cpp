//
// Created by gjk on 2019/12/29.
//

#include "Quote.h"

//override在函数定义中不出现
double Bulk_Quote::net_price(std::size_t n) const {
    if(n>=min_qty)return n*(1-discount)*price;
    return n*price;
}