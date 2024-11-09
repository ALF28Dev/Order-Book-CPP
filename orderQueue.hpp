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
        this->head->setNext(order); // Set dummy head's next to the new highest priority order
    }

    std::shared_ptr<Order> getLowestPriorityOrder() {
        return this->tail;
    }

public:
    OrderQueue() : head(std::make_shared<Order>()), tail(head), size(0), totalVolume(0) {}

    void addOrder(std::shared_ptr<Order> order) {
        if (this->tail == this->head) {
            this->head->setNext(order);
            order->setPrev(this->head);
        } else {
            order->setPrev(this->tail);
            this->tail->setNext(order);
        }
        this->tail = order;
        ++this->size;
        this->totalVolume += order->getSize();
    }

    int getTotalVolume() const {
        return this->totalVolume;
    }

    int getSize() const {
        return this->size;
    }

    std::shared_ptr<Order> getHighestPriorityOrder() const {
        return this->head->getNext();
    }

    std::shared_ptr<Order> removeHighestPriorityOrder() {
        std::shared_ptr<Order> hpo = getHighestPriorityOrder();
        if (!hpo) {
            std::cout << "No Orders In Queue\n";
            return nullptr;
        }

        std::shared_ptr<Order> nhpo = hpo->getNext();
        if (nhpo) {
            nhpo->setPrev(this->head);
        } else {
            this->tail = this->head;
        }

        setHighestPriorityOrder(nhpo);
        --this->size;
        this->totalVolume -= hpo->getSize();

        hpo->setNext(nullptr);
        hpo->setPrev(nullptr);

        return hpo;
    }

    void printAllOrders() const {
        std::shared_ptr<Order> temp = this->head->getNext();
        while (temp) {
            std::cout << "Order ID: " << temp->getOrderID() << "\n";
            temp = temp->getNext();
        }
    }
};
