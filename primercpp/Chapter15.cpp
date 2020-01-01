//
// Created by gjk on 2019/12/29.
//
#include "Quote.h"
#include <iostream>
using namespace std;

int main(){
    Bulk_Quote* bq = new Bulk_Quote("gcb", 15, 3, 0.2);
    Quote* q = bq;
    Bulk_Quote* bbq = dynamic_cast<Bulk_Quote*>(q);
    cout<<bbq->net_price(4)<<endl;
    return 0;
}