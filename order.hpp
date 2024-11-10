#include <stdio.h>
#include <iostream>
using namespace std;

/**
 * @class Order
 * @brief Represents a trading order with details such as time, type, order ID, size, price, and direction.
 * 
 * The `Order` class models the essential properties of a trading order, including unique identifiers,
 * price, quantity, and direction. Additionally, it includes pointers (`prev` and `next`) that allow it
 * to be linked in a doubly-linked list, enabling efficient insertion, deletion, and traversal in data
 * structures that require ordered access to orders, such as an order book.
 *
 * ### Fields:
 * - **time** (`double`): The time of the order, represented in seconds after midnight, with decimal
 *   precision for sub-second accuracy (e.g., milliseconds).
 * - **orderID** (`int`): A unique identifier for each order.
 * - **size** (`int`): The quantity of shares in the order.
 * - **price** (`int`): The price of the order, represented as "dollar price * 10000" for precision
 *   (e.g., $91.14 is represented as 911400).
 * - **direction** (`int`): Specifies the direction of the order, where `1` indicates a buy and `-1` 
 *   indicates a sell.
 * - **type** (`int`): Represents the type of order, encoded as:
 *      - `1`: New limit order
 *      - `2`: Partial cancellation
 *      - `3`: Full deletion
 *      - `4`: Visible limit order execution
 *      - `5`: Hidden limit order execution
 *      - `7`: Trading halt indicator
 * - **prev** (`std::shared_ptr<Order>`): Pointer to the previous order in a linked list, facilitating 
 *   backward traversal.
 * - **next** (`std::shared_ptr<Order>`): Pointer to the next order in a linked list, facilitating 
 *   forward traversal.
 *
 * ### Constructor:
 * - `Order(double time, int type, int orderID, int size, int price, int direction)`:
 *    Initializes all fields and sets `prev` and `next` pointers to `nullptr` by default, creating 
 *    an isolated order that can be linked as needed.
 *
 * This class is marked as `final`, indicating it cannot be inherited from, to preserve its design 
 * as a standalone representation of a trading order.
 */
class Order {
private:
    double time; 
    int orderID;
    int size;
    int price;
    int direction;
    int type;
    Order* next;
    Order* prev;

public:

    virtual ~Order() {}

    Order(double time = 0.0, int order = 1, int id = 0, int size = 0, int price = 0, int direction = 0)
        : time(time), type(order), orderID(id), size(size), price(price), direction(direction),
          next(nullptr), prev(nullptr) {}

    void partialFill(int size) {
        this->size -= size;
    }

    void setNext(Order* next) {
        this->next = next;
    }

    void setPrev(Order* prev) {
        this->prev = prev;
    }

    int getOrderID() {
        return this->orderID;
    }

    double getTime() {
        return this->time;
    }

    int getSize() {
        return this->size;
    }

    int getPrice() {
        return this->price;
    }

    int getDirection() {
        return this->direction;
    }

    int getType() {
        return this->type;
    }
    
    Order* getNext() {
        return this->next;
    }

    Order* getPrev() {
        return this->prev;
    }

    void setPrice(int price) {
        this->price = price;
    }

    void setSize(int size) {
        this->size = size;
    }

    void setOrderType(int type) {
        this->type = type;
    }
};
