#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include "order.hpp"
#include "orderQueue.hpp"
#include "priceLevelTree.hpp"
using namespace std;

/**
 * @class Orderbook
 * @brief Manages and organizes orders in a bid-ask structure for efficient trading operations.
 *
 * The `Orderbook` class represents a trading order book, managing both bids and asks in 
 * separate structures and providing efficient access to the highest-priority orders. 
 * It stores orders by price level and maintains trees for bid and ask prices, allowing 
 * quick insertion, removal, and retrieval operations.
 *
 * The class uses shared and unique pointers for memory management, ensuring that each 
 * order and price level is correctly tracked and managed without manual cleanup.
 *
 * ### Fields:
 * - **bids** (`std::unordered_map<int, std::shared_ptr<OrderQueue>>`): Maps bid price levels to their respective order queues.
 * - **asks** (`std::unordered_map<int, std::shared_ptr<OrderQueue>>`): Maps ask price levels to their respective order queues.
 * - **orderIdMap** (`std::unordered_map<int, std::shared_ptr<Order>>`): Maps unique order IDs to their respective orders for lookup.
 * - **bidTree** (`std::unique_ptr<PriceLevelTree>`): A tree structure organizing bids, allowing quick access to the highest bid.
 * - **askTree** (`std::unique_ptr<PriceLevelTree>`): A tree structure organizing asks, allowing quick access to the lowest ask.
 * - **id** (`int`): A counter to generate unique order IDs.
 *
 * ### Methods:
 * - **getPriceLevel** (`std::unordered_map<int, std::shared_ptr<OrderQueue>>& getPriceLevel(std::shared_ptr<Order> order)`):
 *   Determines whether an order belongs to the bids or asks map based on its direction 
 *   and returns the appropriate map for insertion or lookup.
 * - **addOrderToBook** (`void addOrderToBook(double time, int orderType, int size, int price, int direction)`):
 *   Adds a new order to the order book, updating the price level structure as needed and 
 *   ensuring that orders are tracked by their unique ID.
 * - **removeHighestPriorityOrder** (`std::shared_ptr<Order> removeHighestPriorityOrder()`):
 *   Removes the highest-priority order from the order book, updating internal structures accordingly.
 *
 * The `Orderbook` class is marked as `final`, preventing further inheritance and ensuring 
 * the integrity of the trading logic. It provides robust support for operations typical 
 * in trading environments, such as placing, removing, and tracking orders by ID and price level.
 */
class Orderbook final {
private:
    std::unordered_map<int, std::shared_ptr<OrderQueue>> bids;
    std::unordered_map<int, std::shared_ptr<OrderQueue>> asks;
    std::unordered_map<int, std::shared_ptr<Order>> orderIdMap;
    std::unique_ptr<PriceLevelTree> bidTree;
    std::unique_ptr<PriceLevelTree> askTree;
    int id;

public:
    Orderbook() : bidTree(std::make_unique<PriceLevelTree>()), askTree(std::make_unique<PriceLevelTree>()), id(1) {}

    std::unordered_map<int, std::shared_ptr<OrderQueue>>& getPriceLevel(std::shared_ptr<Order> order) {
        int price = order->getPrice();
        if (order->getDirection() == 1) {
            this->bidTree->insert(price);
            return bids; // return bids price level.
        }
        this->askTree->insert(price);
        return asks; // return asks price level;
    }



    void addOrderToBook(double time = 0.0, int orderType = 1, int size = 0, int price = 0, int direction = 0) {
        std::shared_ptr<Order> order = std::make_shared<Order>(time, orderType, this->id, size, price, direction);

        if (orderType == 1) {
            // market order immediately executed against the best available price in the opposite side of the order book (bids for sell orders, asks for buy orders).
            executeMarketOrderAgainstBook(order);
        } else if (orderType == 2) {
            // limit order added to the book immediately at the specified price.
            addOrderToBook(order);
        } 
        /*else if (orderType == 3) {
            // stop order added to the book when price level reached (execute like market order when price level hit).
        } else if (orderType == 4) {
            // stop limit order which gets added to book at specified price when target price hit.
        } else if (orderType == 5) {
            // fill or kill market order, use current price level to determine if adequate volume is available to fill order or cancel.
        }*/
        
    }

    void addOrderToBook(std::shared_ptr<Order> order) {
        int price = order->getPrice();
        std::unordered_map<int, std::shared_ptr<OrderQueue>>& priceLevel = getPriceLevel(order);
        if (!priceLevel.count(price)){
            priceLevel[price] = std::make_shared<OrderQueue>(); // Create level if not in level map.
        }
        priceLevel[price]->addOrder(order); // Add order at price level.
        cout << "Order Added to Book (ID: " << this->id << " Type: " << order->getType() << " Size: " << order->getSize() << " Price: " << price << " Direction: " << order->getDirection() << ")\n";
        orderIdMap[id] = order;
        ++this->id;
    }


    bool canMatchOrders() {
        // check price levels containing orders exist in the bid and ask tree and ensure the max bid is greater than the min ask.
        return (!this->bidTree->isEmpty() && !this->askTree->isEmpty() && this->bidTree->max() >= this->askTree->min());
    }

    void removeEmptyPriceLevels(int highestBid, int lowestAsk) {
        // remove price level from tree as no orders exist at that level
        if (!this->bids[highestBid]->getSize()) {
            this->bidTree->remove(highestBid);
        }
        if (!this->asks[lowestAsk]->getSize()) {
            this->askTree->remove(lowestAsk);
        }
    }

    void updateOrderSize(int id, int size) {
        this->orderIdMap[id]->setSize(size);
    }

    int restingOrderExecutionPrice(std::shared_ptr<Order> bidOrder, std::shared_ptr<Order> askOrder) {
        // Get the execution price for the order which was first in the book.
        return (bidOrder->getOrderID() < askOrder->getOrderID()) ? bidOrder->getPrice() : askOrder->getPrice();
    }

    void executeMarketOrderAgainstBook(std::shared_ptr<Order> order) {
        // Loop till order filled or no more orders in book.
        // Slippage can occur when we bump up or down price levels to fill the order.

        bool isBidOrder = (order->getDirection() == 1);
        auto& targetTree = isBidOrder ? this->askTree : this->bidTree;
        auto& targetQueue = isBidOrder ? this->asks : this->bids;
        bool orderFilled = false;

        while ( !targetTree->isEmpty() ) { 
            // Continue while price levels exist. CALCULATE AVERAGE EXECUTION PRICE.

            bool bidCondition = isBidOrder && order->getPrice() >= this->askTree->min();
            bool askCondition = !isBidOrder && this->bidTree->max() >= order->getPrice();

            if (!(bidCondition || askCondition)) {
                // Break when order matching cannot occur EVEN if price levels exist.
                break;
            }

            int bestPrice = isBidOrder ? targetTree->max() : targetTree->min(); // Get the best price (min for ask, max for bid)
            std::shared_ptr<OrderQueue> bestQueue = targetQueue[bestPrice];
            std::shared_ptr<Order> bestOrder = bestQueue->getHighestPriorityOrder();

            if (order->getSize() > bestOrder->getSize()) {
                // Market order size is greater than the best price level order size (partial fill of market order)
                // Remove fully filled best order
                order->partialFill(bestOrder->getSize());
                bestQueue->removeHighestPriorityOrder();
            } else if (order->getSize() < bestOrder->getSize()) {
                // Market order size is less than the best price level order size (partial fill of best order)
                bestOrder->partialFill(order->getSize());
                orderFilled = true;
            } else {
                // Market order size equals the best price level order size (full fill for both)
                bestQueue->removeHighestPriorityOrder();
                orderFilled = true;
            }

            if (bestQueue->getSize() == 0) {
                // Remove empty price levels from the target tree
                targetTree->remove(bestPrice);
                targetQueue.erase(bestPrice);
            }
        }

        if ( orderFilled ) {
            cout << "MARKET ORDER FILLED\n";
        } else {
            // Worst case no more volume in the entire book.
            // Add remaining order to book as limit order for fill when more orders hit book.
            cout << "MARKET ORDER PARTIAL FILLED\n"; 
            order->setOrderType(2);
            addOrderToBook(order);    
        }
    }

    void matchOrders() {
        while (canMatchOrders()) {

            int highestBid = this->bidTree->max();
            int lowestAsk = this->askTree->min();

            // this section needs to be updated to store stop erders also get considered.
            // by also looking through the stop orders at this level to see whats the highest priority.
            

            // modify four lines below to get the highest priority order in the system
            // currently it gets limit orders as market orders immediately get matched best price
            // must get highest priority order out of all limit orders and stop orders etc. 
            std::shared_ptr<OrderQueue> bidQueue = this->bids[highestBid];
            std::shared_ptr<OrderQueue> askQueue = this->asks[lowestAsk];    
            std::shared_ptr<Order> bidOrder = bidQueue->getHighestPriorityOrder();
            std::shared_ptr<Order> askOrder = askQueue->getHighestPriorityOrder();

            int bidOrderId = bidOrder->getOrderID();
            int askOrderId = askOrder->getOrderID();

            int executionPrice = restingOrderExecutionPrice(bidOrder, askOrder);

            if (bidOrder->getSize() == askOrder->getSize()) {
                // Both Bid and Ask match in size and can be removed.
                bidQueue->removeHighestPriorityOrder();
                askQueue->removeHighestPriorityOrder();
                cout << "Bid Order: " << bidOrderId << " matched with Ask Order: " << askOrderId << " at Price: " << executionPrice << "\n";
            } else if (bidOrder->getSize() > askOrder->getSize()) {
                // Ask filled fully, Bid partial fill.
                bidOrder->partialFill(askOrder->getSize());
                askQueue->removeHighestPriorityOrder(); 
                cout << "Bid Order Partial Fill: " << bidOrderId << " matched with Ask Order: " << askOrderId << " at Price: " << executionPrice << "\n";
            } else {
                // Bid filled fully, Ask partial fill.
                askOrder->partialFill(bidOrder->getSize());
                bidQueue->removeHighestPriorityOrder();
                cout << "Bid Order: " << bidOrderId << " matched with Ask Order Partial Fill: " << askOrderId << " at Price: " << executionPrice << "\n";
            }
            removeEmptyPriceLevels(highestBid, lowestAsk);
        }
    }
};

int main() {
    std::unique_ptr<Orderbook> book = std::make_unique<Orderbook>();

    
    book->addOrderToBook(123.2, 2, 7, 210, -1);
    book->addOrderToBook(123.3, 2, 6, 210, -1);
    book->addOrderToBook(123.3, 2, 5, 210, 1);
    book->addOrderToBook(123.3, 2, 4, 210, -1);
    book->addOrderToBook(123.3, 1, 22, 214, 1);
    
    book->matchOrders();
    cout << "* Finished Matching *" << "\n";

    return 0;
}