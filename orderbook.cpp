#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "order.hpp"
#include "orderQueue.hpp"
#include "priceLevelTree.hpp"
#include "orderType.hpp"
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
    std::unordered_map<int, OrderQueue*> bids;
    std::unordered_map<int, OrderQueue*> asks;
    std::unordered_map<int, Order*> orderIdMap;
    PriceLevelTree* bidTree;
    PriceLevelTree* askTree;
    int id;

public:
    Orderbook() : bidTree(new PriceLevelTree()), askTree(new PriceLevelTree()), id(1) {}

    void addToBook(Order* order) {
        int price = order->getPrice();
        std::unordered_map<int, OrderQueue*>& priceLevel = getPriceLevel(order);
        if (!priceLevel.count(price)){
            priceLevel[price] = new OrderQueue(); // Create level if not in level map.
        }
        priceLevel[price]->addOrder(order); // Add order at price level.
        std::cout << "Order Added to Book (ID: " << this->id << " Type: " << order->getType() << " Size: " << order->getSize() << " Price: " << price << " Direction: " << order->getDirection() << ")\n";
        orderIdMap[id] = order;
        ++this->id;
    }

    void visualise() {
        std::cout << "ASKS:\n----------------------\n";
        int cumulativeVolume;
        std::set<int>* allAskPriceLevels = this->askTree->getAllLevels();
        cumulativeVolume = 0;
        for (std::set<int>::reverse_iterator it = allAskPriceLevels->rbegin(); it != allAskPriceLevels->rend(); ++it) {
            cumulativeVolume += this->asks[*it]->getTotalVolume();
        }
        for (std::set<int>::reverse_iterator it = allAskPriceLevels->rbegin(); it != allAskPriceLevels->rend(); ++it) {
            int levelVolume = this->asks[*it]->getTotalVolume();
            std::string output(cumulativeVolume, '#');
            std::cout << *it << " " << output << "\n";
            cumulativeVolume -= levelVolume;
        }
        std::cout << "-\n";
        std::set<int>* allBidPriceLevels = this->bidTree->getAllLevels();
        
        cumulativeVolume = 0;
        for (std::set<int>::reverse_iterator it = allBidPriceLevels->rbegin(); it != allBidPriceLevels->rend(); ++it) {
            cumulativeVolume += this->bids[*it]->getTotalVolume();
            std::string output(cumulativeVolume, '#');
            std::cout << *it << " " << output << "\n";
        }
        std::cout << "----------------------\nBIDS:\n";
    }

    void addOrderToBook(double time = 0.0, int orderType = 1, int size = 0, int price = 0, int direction = 0) {
        Order* order = new Order(time, orderType, this->id, size, price, direction);
        if (orderType == 1) {
            // market order immediately executed against the best available price in the opposite side of the order book (bids for sell orders, asks for buy orders).
            executeMarketOrderAgainstBook(order);
        } else if (orderType == 2 || orderType == 3 || orderType == 4) {
            // Limit Order
            // Stoplimit order only hits the book when (perhaps modify to add to seperate queue so it never hits the main book)
            // Fill or Kill (FOK) Limit Order
            addToBook(order);
        }
    }

    std::unordered_map<int, OrderQueue*>& getPriceLevel(Order* order) {
        int price = order->getPrice();
        if (order->getDirection() == 1) {
            this->bidTree->insert(price);
            return bids; // return bids price level.
        }
        this->askTree->insert(price);
        return asks; // return asks price level;
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

    void executeMarketOrderAgainstBook(Order* order) {
        // Loop till order filled or no more orders in book.
        // Slippage can occur when we bump up or down price levels to fill the order.

        bool isBidOrder = (order->getDirection() == 1);
        PriceLevelTree*& targetTree = isBidOrder ? this->askTree : this->bidTree;
        std::unordered_map<int, OrderQueue*>& targetQueue = isBidOrder ? this->asks : this->bids;
        bool orderFilled = false;

        std::cout << "Market Order Size: " << order->getSize() << "\n";

        int originalSize = order->getSize();

        while (!orderFilled) { 
            // Continue sweeping while price levels exist. CALCULATE AVERAGE EXECUTION PRICE.
            bool bidCondition = isBidOrder && order->getPrice() >= this->askTree->min();
            bool askCondition = !isBidOrder && this->bidTree->max() >= order->getPrice();

            if (!(bidCondition || askCondition)) {
                // Break when order matching cannot occur EVEN if price levels exist.
                break;
            }

            int bestPrice = isBidOrder ? targetTree->min() : targetTree->max(); // Get the best price (min for ask, max for bid)
            
            Order* bestOrder = targetQueue[bestPrice]->getHighestPriorityOrder();
            
            int executionPrice = restingOrderExecutionPrice(order, bestOrder);
            if (order->getSize() > bestOrder->getSize()) {
                // Market order size is greater than the best price level order size (partial fill of market order)
                // Remove fully filled best order
                order->partialFill(bestOrder->getSize());
                targetQueue[bestPrice]->removeHighestPriorityOrder();
                std::cout << "Market order partial fill and best match filled, BEST MATCH SIZE: " << bestOrder->getSize() << " ORDER SIZE: " << order->getSize() << "\n";
            } else if (order->getSize() < bestOrder->getSize()) {
                // Market order size is less than the best price level order size (partial fill of best order)
                targetQueue[bestPrice]->partialFill(bestOrder, order->getSize());
                orderFilled = true;
                std::cout << "Market order and best match partial filled, BEST MATCH SIZE: " << bestOrder->getSize() << " ORDER SIZE: " << order->getSize() << "\n";
            } else {
                // Market order size equals the best price level order size (full fill for both)
                targetQueue[bestPrice]->removeHighestPriorityOrder();
                orderFilled = true;
                std::cout << "Market order and best match filled\n";
            }

            if (targetQueue[bestPrice]->getTotalVolume() == 0) {
                // Remove empty price levels from the target tree
                targetTree->remove(bestPrice);
                targetQueue.erase(bestPrice);
            }
        }

        if ( !orderFilled ) {
            if (order->getSize() < originalSize) {
                std::cout << "Market Order Partial Fill\n"; 
            } else {
                std::cout << "Market Order Not Filled - Added To Book as Limit Order\n"; 
            }
            order->setOrderType(2);
            addToBook(order);    
        } else {
            std::cout << "Market Order Filled\n"; 
        }
    }

    bool handleFillOrKill(OrderQueue* orderQueue, Order* order, OrderQueue* queue) {
        if (order->getType() == 4 && queue->getTotalVolume() < order->getSize()) {
            // Order is bigger than total volume at best price level on other side.
            orderQueue->removeHighestPriorityOrder();
            std::cout << "FOK Order: " << order->getOrderID() << " Cancelled, Reason: Insufficient Volume\n";
            return true;
        }
        return false;
    }

    int restingOrderExecutionPrice(Order* bidOrder, Order* askOrder) {
        // Get the execution price for the order which was first in the book.
        return (bidOrder->getOrderID() < askOrder->getOrderID()) ? bidOrder->getPrice() : askOrder->getPrice();
    }

    bool canMatchOrders() {
        // check price levels containing orders exist in the bid and ask tree and ensure the max bid is greater than the min ask.
        return (!this->bidTree->isEmpty() && !this->askTree->isEmpty() && this->bidTree->max() >= this->askTree->min());
    }

    void matchOrders() {
        while (canMatchOrders()) {            
            int highestBid = this->bidTree->max();
            int lowestAsk = this->askTree->min();

            OrderQueue* bidQueue = this->bids[highestBid];
            OrderQueue* askQueue = this->asks[lowestAsk];

            Order* bidOrder = bidQueue->getHighestPriorityOrder();
            Order* askOrder = askQueue->getHighestPriorityOrder();

            if (handleFillOrKill(bidQueue, bidOrder, askQueue) || handleFillOrKill(askQueue, askOrder, bidQueue)) {
                removeEmptyPriceLevels(highestBid, lowestAsk);
                continue;
            }

            int bidOrderId = bidOrder->getOrderID();
            int askOrderId = askOrder->getOrderID();

            int executionPrice = restingOrderExecutionPrice(bidOrder, askOrder);

            if (bidOrder->getSize() == askOrder->getSize()) {
                // Both Bid and Ask match in size and can be removed.
                bidQueue->removeHighestPriorityOrder();
                askQueue->removeHighestPriorityOrder();
                std::cout << "Bid Order: " << bidOrderId << " matched with Ask Order: " << askOrderId << " at Price: " << executionPrice << "\n";
            } else if (bidOrder->getSize() > askOrder->getSize()) {
                // Ask filled fully, Bid partial fill.
                bidQueue->partialFill(bidOrder, askOrder->getSize());
                askQueue->removeHighestPriorityOrder(); 
                std::cout << "Bid Order Partial Fill: " << bidOrderId << " matched with Ask Order: " << askOrderId << " at Price: " << executionPrice << "\n";
            } else {
                // Bid filled fully, Ask partial fill.
                askQueue->partialFill(askOrder, bidOrder->getSize());
                bidQueue->removeHighestPriorityOrder();
                std::cout << "Bid Order: " << bidOrderId << " matched with Ask Order Partial Fill: " << askOrderId << " at Price: " << executionPrice << "\n";
            }
            removeEmptyPriceLevels(highestBid, lowestAsk);
        }
        std::cout << "* Finished Matching *" << "\n";
    }
};

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
    book->addOrderToBook(123.2, ORDER_LIMIT, 1, 215, 1);
    book->addOrderToBook(123.3, ORDER_LIMIT, 2, 214, 1);
    book->addOrderToBook(123.4, ORDER_LIMIT, 4, 213, 1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 6, 212, 1);
    book->addOrderToBook(123.2, ORDER_LIMIT, 1, 214, -1);
    book->addOrderToBook(123.3, ORDER_LIMIT, 3, 215, -1);
    book->addOrderToBook(123.4, ORDER_LIMIT, 7, 216, -1);
    book->addOrderToBook(123.5, ORDER_LIMIT, 5, 213, 1);


    book->matchOrders();
    book->visualise();

    book->addOrderToBook(123.5, ORDER_MARKET, 5, 213, -1);


    book->visualise();

    return 0;
}