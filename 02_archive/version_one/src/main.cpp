#include <iostream>
#include <orderbook/orderbook.hpp>

int main() {

    std::unique_ptr<Orderbook> book = std::make_unique<Orderbook>();
    book->addOrderToBook(123.5, ORDER_LIMIT, 4, 211, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 2, 214, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 3, 214, -1);
    book->addOrderToBook(123.2, ORDER_LIMIT, 7, 210, -1);
    book->addOrderToBook(123.3, ORDER_LIMIT, 6, 210, -1);
    book->addOrderToBook(123.4, ORDER_LIMIT, 5, 210, 1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 1, 210, -1);
    book->addOrderToBook(123.6, ORDER_MARKET, 22, 214, 1);
    book->addOrderToBook(123.7, ORDER_FILL_OR_KILL, 20, 214, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 3, 210, -1);
    book->addOrderToBook(123.6, ORDER_MARKET, 22, 214, 1);
    book->addOrderToBook(123.7, ORDER_FILL_OR_KILL, 20, 214, -1);
    book->addOrderToBook(123.6, ORDER_LIMIT, 22, 214, 1);
    book->addOrderToBook(123.7, ORDER_LIMIT, 20, 214, -1);
    book->addOrderToBook(123.2, ORDER_STOP_LIMIT, 1, 215, 1, 217);
    book->addOrderToBook(123.3, ORDER_LIMIT, 2, 214, 1);
    book->addOrderToBook(123.4, ORDER_LIMIT, 4, 213, 1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 6, 212, 1);
    book->addOrderToBook(123.2, ORDER_LIMIT, 1, 214, -1);
    book->addOrderToBook(123.3, ORDER_LIMIT, 3, 215, -1);
    book->addOrderToBook(123.4, ORDER_LIMIT, 7, 216, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 5, 213, 1);
    book->matchOrders();

    book->addOrderToBook(123.5, ORDER_MARKET, 5, 213, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 2, 210, 1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 4, 209, 1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 2, 209, 1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 3, 207, 1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 4, 206, 1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 6, 206, 1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 5, 206, 1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 1, 213, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 2, 215, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 3, 216, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 3, 217, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 4, 217, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 5, 218, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 6, 218, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 7, 219, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 7, 220, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 8, 221, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 8, 222, -1);
    book->matchOrders();

    book->visualise();
    book->visualiseChart();

    return 0;
}