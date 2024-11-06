#include <stdio.h>
#include <iostream>
using namespace std;

/**
 * @class OrderQueue
 * @brief Manages a queue of orders, allowing efficient insertion, removal, and traversal.
 *
 * The `OrderQueue` class represents a doubly-linked list structure that maintains the sequence 
 * of orders at a specific price level. It provides efficient access to both the front (highest 
 * priority) and back (lowest priority) of the queue, making it suitable for use in an order book 
 * where orders are prioritized by time at each price level.
 *
 * This class uses `std::shared_ptr` for `head` and `tail` pointers to manage memory automatically 
 * and ensure safe access to the first and last orders in the queue.
 *
 * ### Fields:
 * - **head** (`std::shared_ptr<Order>`): A pointer to the first order in the queue, representing 
 *   the highest priority order at the price level.
 * - **tail** (`std::shared_ptr<Order>`): A pointer to the last order in the queue, representing 
 *   the lowest priority order at the price level.
 * - **size** (`int`): The current number of orders in the queue, which helps track the depth of 
 *   the order book at this price level.
 *
 * ### Methods:
 * - **addOrder** (`void addOrder(std::shared_ptr<Order> order)`): Adds a new order to the back of 
 *   the queue, updating the `tail` pointer and incrementing the `size`.
 * - **removeOrder** (`std::shared_ptr<Order> removeOrder()`): Removes the highest priority order 
 *   from the front of the queue, updating the `head` pointer and decrementing the `size`.
 * - **getSize** (`int getSize() const`): Returns the number of orders in the queue.
 * - **getHighestPriorityOrder** (`std::shared_ptr<Order> getHighestPriorityOrder() const`): 
 *   Returns the order at the front of the queue without removing it.
 * - **getLowestPriorityOrder** (`std::shared_ptr<Order> getLowestPriorityOrder() const`): 
 *   Returns the order at the back of the queue without removing it.
 *
 * The `OrderQueue` class is marked as `final`, indicating it cannot be inherited from, to 
 * maintain its intended functionality as a standalone queue for managing orders at a single 
 * price level.
 */
class OrderQueue final {
private:
    std::shared_ptr<Order> head;
    std::shared_ptr<Order> tail;
    int size;
    int totalVolume;

    void setHighestPriorityOrder(std::shared_ptr<Order> order) {
        this->head->setNext(order); // set dummy head next to npho.
    }

    std::shared_ptr<Order> getLowestPriorityOrder() {
        return this->tail;
    }

public:
    OrderQueue() : head(std::make_shared<Order>()), tail(head), size(0) {}

    void addOrder(std::shared_ptr<Order> order) {
        order->setPrev(this->tail); // new order prev is current tail.
        this->tail->setNext(order); // current tail next is new order.
        this->tail = order; // set tail to the new order.
        ++this->size;
        this->totalVolume += order->getSize();
    }

    int getTotalVolume() {
        return this->totalVolume;
    }

    int getSize() {
        return this->size;
    }

    std::shared_ptr<Order> getHighestPriorityOrder() {
        return this->head->getNext();
    }

    std::shared_ptr<Order> removeHighestPriorityOrder() {
        std::shared_ptr<Order> hpo = getHighestPriorityOrder();
        if (!hpo) {    
            cout << "No Orders In Queue\n"; // no orders in queue.
            return nullptr;
        }   
        std::shared_ptr<Order> nhpo = hpo->getNext(); // get next hpo afte curr hpo.
        if (nhpo) {
            nhpo->setPrev(this->head); // set its prev to the dummy head if not nullptr.
        }
        setHighestPriorityOrder(nhpo); 
        --this->size;
        this->totalVolume -= hpo->getSize();
        return hpo;
    }

    void printAllOrders() {
        std::shared_ptr<Order> temp = this->head->getNext();
        while (temp) {
            cout << "Order ID: " << temp->getOrderID() << "\n";
            temp = temp->getNext();
        }
    }
};