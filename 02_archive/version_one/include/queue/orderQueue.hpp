#include <stdio.h>
#include <iostream>

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
    Order* dummyHead;
    Order* tail;
    int size;
    int* totalVolume;

    Order* getLowestPriorityOrder() {
        return this->tail;
    }

public:
    OrderQueue() : dummyHead(new Order()), tail(dummyHead), size(0), totalVolume(new int(0)) {}

    ~OrderQueue() {
        delete dummyHead;       // Clean up dummy head
        delete totalVolume; // Clean up dynamically allocated totalVolume
    }

    void addOrder(Order* order) {
        if (this->tail == this->dummyHead) { // If the list is empty
            this->dummyHead->setNext(order);  // Dummy head's next points to the new order
            order->setPrev(this->dummyHead);  // New order's prev is the dummy head
        } else {                         // If the list has other orders
            order->setPrev(this->tail);  // New order's prev is current tail
            this->tail->setNext(order);  // Current tail's next is the new order
        }
        this->tail = order;              // Update tail to the new order
        ++this->size;
        *totalVolume += order->getSize();
    }

    int getTotalVolume() const {
        return *totalVolume;
    }

    int getSize() const {
        return this->size;
    }

    void partialFill(Order* order, int size) {
        order->partialFill(size);
        *totalVolume -= size;
    }

    Order* getHighestPriorityOrder() const {
        return this->dummyHead->getNext();
    }

    Order* removeHighestPriorityOrder() {
        Order* hpo = getHighestPriorityOrder();
        if (!hpo) {
            std::cout << "No Orders In Queue\n";
            return nullptr;
        }

        Order* nhpo = hpo->getNext(); // Get next highest priority order after current HPO
        if (nhpo) {
            nhpo->setPrev(this->dummyHead); // Set its prev to the dummy head if not nullptr
        } else {
            this->dummyHead->setNext(nullptr);
            this->tail = this->dummyHead;   // Reset tail to dummy head if no more orders
        }
        this->dummyHead->setNext(nhpo);
        --this->size;
        *totalVolume -= hpo->getSize();

        // Disconnect hpo from the list
        hpo->setNext(nullptr);
        hpo->setPrev(nullptr);

        return hpo;
    }

    void printAllOrders() const {
        Order* temp = this->dummyHead->getNext();
        std::cout << "CALLING ALL ORDERS FUNC\n";
        while (temp) {
            std::cout << "Order ID: " << temp->getOrderID() << "\n";
            temp = temp->getNext();
        }
    }
};
