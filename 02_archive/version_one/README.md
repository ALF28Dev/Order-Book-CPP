# Order-Book-CPP
![GNU Plot of Order book](./images/example-orderbook-chart.png)

Hey everyone, thank you for taking the time to view this repository. This is my C++ based order book project, which I used as an opportunity to learn some basic C++ and fundamental concepts related to market microstructure. The project aims to replicate core functionalities of one of the cornerstone components of an exchange: the order book. I used the included [LOBSTER](https://lobsterdata.com/) readme txt file to learn about what information is associated with every order in an order book on an exchange. *Additional Note: This project isn't currently a low latency project.*

## Book Structure

![Order book structure diagram](./images/orderbook-structure.png)
*The diagram above demonstrates the structure of the order book where two Tree structures are used to store the price levels on the Bid & Ask sides of the book. A FIFO Queue exists which holds the orders at that specific price level in a manner which priorites them in a first come, first served basis.*

- **Price Levels - Red-Black Trees**
    - A Set structure is used to store the Bid & Ask price levels within the book. These price levels are stored as integers. The Set structure allows for efficient retrieval of the best (highest) Bid and (lowest) Ask price levels.
    - A Set structure was chosen specifically as its [implemented using a Red-Black tree in C++](https://en.cppreference.com/w/cpp/container/set). A Red-Black tree is an ideal ordered structure for managing price levels in the order book. Its tree structure enables efficient retrieval of the best Bid (Max) and Ask (Min) price levels (**O(1) time**), while its self-balancing properties ensure **O(logn) time** complexity for search, insert, and delete operations.

- **Price Levels Mapped to FIFO Queues - Unordered Map**
    - Each price level in the Tree structure is added to an Unordered Map, which links each price level (key) to a FIFO Doubly Linked List Queue (value).
    - Once we retrieve the best Bid or Ask in **O(1) time** from the Tree structure, we can access the orders at this price level in **O(1) time** through the Unordered Map.


- **FIFO Queues - Doubly Linked Lists**
    - Every order in the book is added to the Doubly Linked List held at the price level corresponding to that order. When an order arrives, we look up the corresponding FIFO queue using the Unordered Map in **O(1) time** and append it to the back to ensure that orders arriving first are matched first.
    - The Doubly Linked List structure has a pointer to the head, which enables access to the highest priority order (first) in **O(1) time**, and new orders can be added to the end of the list in **O(1) time** using the pointer to the tail.


- **Orders**
    - An Order class has been created to hold all of the data associated with an order. An order class allows us to formally structure the attributes of an order, including:
        - Price
        - Type
        - Direction/Side
        - Size
        - ID
        - Time of arrival
        - Next & Prev pointers for the FIFO Queues
    - Not all orders require the same data, and some require additional fields. To address this, the `Order` class is designed as a parent class from which additional child `Order` classes can be derived.
    - For example, a `LimitOrder` class has been defined, which allows the creation of Stop Limit Orders with an additional price field. Once the original target price field is reached in the book, the Limit Order is moved to its target price in the order book.


## Matching Engine
- **Limit Order Matching**
    - **Order Matching Conditions**
        - The Limit order matching system is implemented within the function `matchOrders()`. This function matches Orders within the book while the book satisfies the conditions for matching orders. This condition requires orders to exist on both sides of the book & the Max Bid >= Min Ask. This means someone is willing to pay more than the lowest price which someone is willing to pay, therefore we can perform a match.
    - **Fill Types (Full Fill & Partial Fill)**
        - The book structure I have created supports both full and partial fills.
        - If the size of the order on one side of the book is less than the order on the other side with which it is being matched, the smaller order will be fully filled, while the larger order will be partially filled, with the remaining unmatched quantity staying in the order book.
        - **Handling Partial Fills**
            - When an order is partially filled it remains in the book in the same position within the FIFO Queue except its size is updated to reflect how much of the order has been filled.
        - **Handling Full Fills**
            - When an order is filled it is removed from the book. It is taken out of the FIFO queue and a new order takes its place as the highest priority.
            - If no other orders exist at that price level, the price level is removed from the Set (Red-Black Tree) to ensure the Min & Max functions always retrieve the best price levels containing orders.
    - **Handling Stop Limit Orders**
        - During order matching we always fetch the best Bid & Ask when the conditions for matching are met. If either of these orders is a Stop Limit Order we first check to see if it has already been moved to its correct position in the book. If it has not, we update the price field of the order to reflect the target price set by the user, and move the order to the correct position in the book.
    - **Handling Fill-Or-Kill Orders**
        - If either of the best Bid & Ask orders is a Fill-Or-Kill order we perform an additional check which ensures that the best price level on the other side of the book contains sufficient available volume to fill the order. If not, we remove the order from the book and do not perform a match.
    - **Execution Price**
        - When a match occurs the resting order execution price is used as the execution price. This dictates that the execution price between two matched orders should be the price of the order which was first in the book.

- **Market Order Execution**
    - Market order execution occurs as soon as a market order hits the book. Upon hitting the book, the Market order is executed against the best price level on the other side of the book until it is filled or until there are no available orders on the other side of the book to match against.

- **Sweeping & Slippage**
    - During the order matching process for both Limit & Market order execution we sweep the book. We always fetch the best price level for both Bid & Ask orders. 
    - Large orders may consume all of the volume at the best price level on the other side of the book leading to slippage as we need to move to lower/higher price levels to fill the remaining size of the order which couldn't be filled at the best price level. 
    - This process is known as sweeping and the outcome of sweeping is slippage, where the user doesn't always recieve the best price.

## Types of Orders Accepts
- **Market Orders**
    - Market orders immediately get executed at the best price level on the other side of the book.
- **Limit Orders**
    - Limit orders get added to the book at the specified price level and remain in the book until they are executed.
- **Stop Limit Orders**
    - Stop Limit orders get added to the book at a specified price level with an additional target price level. When the specified price level is reached by the book the order is moved and added to the book at the target price level. It remains in this price level until it gets executed.
- **Fill or Kill Orders**
    - Fill or Kill orders are limit orders which are added to the book at a specified price. When the price level is reached and the order is ready to be matched we first check to ensure the best price level on the other side of the book has sufficient volume to fill the entire order, otherwise we remove it from the book.

## Visualising the Book
- Shows Cumulative Bid/Ask orders strating from the best Bid/Ask to worst Bid/Ask
    - **Cmd Output**
        - The method `visualise()` within the orderbook class allows for the state of the book at a specific moment in the to be printed to the cmd.
        - ![CMD Order Book Chart](./images/example-cmd-orderbook-output.png)
    - **GNU Plot Chart**
        - The method `visualiseChart()` within the orderbook clss shows the state of the book at a specific moment in time within a formal GNU Bar chart Plot like shown in the image above.

## Missing Pieces & Future Improvements
- Thoroughly learn C++ best practices to improve the existing codebase and reduce latency.
- Convert the time when an order is added to the book into "seconds after midnight with decimal precision of at least milliseconds".
- Build a cutom Red Black Tree Class inspired by the underlying mechanism within the Linux OS which structures processes in a Red-Black Tree Structure (set implementation for now since the underlying structure is a Red-Black Tree).
- Lock free queue implementation for lower latency.
- Multithreading (Must consider Thread Safety).
- Metrics such as order flow, liquidity ratios, fill rate.
- Update the chart class to have a more appropriate label in the mid point between the best Bid & Ask instead of -1.

