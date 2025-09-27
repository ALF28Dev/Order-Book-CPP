#include <stdio.h>
#include <iostream>
#include <set>

/**
 * @class PriceLevelTree
 * @brief Manages and organizes price levels in a binary search tree structure for fast retrieval.
 *
 * The `PriceLevelTree` class represents a data structure that stores and organizes price levels 
 * in a balanced binary search tree (using `std::set`). This structure provides efficient access 
 * to the highest and lowest prices, making it suitable for managing bid and ask levels in a 
 * trading order book.
 *
 * The `tree` field stores unique integer price levels in sorted order, allowing quick insertion, 
 * deletion, and access to the minimum and maximum prices. This structure supports fast access to 
 * the best bid or ask levels.
 *
 * ### Fields:
 * - **tree** (`std::set<int>`): A balanced binary search tree that stores unique price levels in 
 *   ascending order. The lowest price can be accessed with `*tree.begin()`, and the highest price 
 *   with `*tree.rbegin()`.
 *
 * ### Methods:
 * - **insert** (`void insert(int price)`): Inserts a new price level into the tree.
 * - **remove** (`void remove(int price)`): Removes a price level from the tree.
 * - **getLowestPrice** (`int getLowestPrice() const`): Returns the lowest price level in the tree.
 * - **getHighestPrice** (`int getHighestPrice() const`): Returns the highest price level in the tree.
 * - **isEmpty** (`bool isEmpty() const`): Checks if the tree has any price levels.
 *
 * The `PriceLevelTree` class is marked as `final`, indicating it cannot be inherited from. 
 * This enforces its intended use as a standalone utility for managing price levels and 
 * accessing them in a trading context.
 */
class PriceLevelTree final {
private:
    std::set<int> tree;

public:
    void insert(int price) {
        this->tree.insert(price);
    }

    void remove(int price) {
        this->tree.erase(price);
    }

    int min() const {
        return *this->tree.begin();
    }

    int max() const {
        return *this->tree.rbegin();
    }

    bool isEmpty() const {
        return this->tree.empty();
    }

    int getDepth() {
        return this->tree.size();
    }

    std::set<int>* getAllLevels() {
        return &this->tree;
    }
};
