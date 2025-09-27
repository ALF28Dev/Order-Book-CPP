
#include "orderbook/orderbook/orderbook.h"

int main()
{
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->match_orders();
    ob->~book();
    return 0;
}
