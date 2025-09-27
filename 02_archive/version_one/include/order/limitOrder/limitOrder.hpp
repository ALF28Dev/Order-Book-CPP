#include <stdio.h>
#include <iostream>
#include "order/order.hpp"

class limitOrder : public Order
{

private:
    int limitPrice;
    bool limitHit;

public:
    limitOrder(double time = 0.0, int order = 1, int id = 0, int size = 0, int price = 0, int direction = 0, int limitPrice = 0) 
        : limitPrice(limitPrice), limitHit(false), Order(time, order, id, size, price, direction) {}

    int getLimitPrice() {
        return this->limitPrice;
    }

    bool hasLimitBeenHit() {
        return this->limitHit;
    }

    void updatePrice(int price) {
        this->limitHit = true;
        this->setPrice(price);
    }
};