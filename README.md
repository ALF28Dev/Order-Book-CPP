# Order-Book-CPP
![GNU Plot of Order book](./images/example-orderbook-chart.png)

Hey everyone, thank you for taking the time to view this repository. This is my C++ based order book project, which I used as an opportunity to learn some basic C++ and fundamental concepts related to market microstructure. The project aims to replicate core functionalities of one of the cornerstone components of an exchange: the order book. I used the included [LOBSTER](https://lobsterdata.com/) readme txt file to learn about what information is associated with every order in an order book on an exchange.

## Book Structure

![Order book structure diagram](./images/orderbook-structure.png)
*The diagram above is demonstrating the structure of the order book where two Tree Structures are used to store the price levels on the Bid & Ask sides of the book. A FIFO Queue exists which holds the orders at that specific price level in a manner which priorites them in a first come, first served basis.*

- **Price Levels - Red-Black Trees**
    - A Set structure is used to store the Bid & Ask price levels within the book. These price levels are stored as integers. The Set structure allows for efficient retrieval of the best (highest) Bid and (lowest) Ask price levels.
    - A Set structure was chosen specifically as its [implemented using a Red-Black tree in C++](https://en.cppreference.com/w/cpp/container/set). A Red-Black tree is an ideal ordered structure for managing price levels in the order book. Its tree structure enables efficient retrieval of the best Bid (Max) and Ask (Min) price levels (**O(1) time**), while its self-balancing properties ensure **O(logn) time** complexity for search, insert, and delete operations.

- **Price Levels Mapped to FIFO Queues - Unordered Map**
    - Each of the price levels in the Tree structure is added to an Unordered Map, which links each price level (key) to a FIFO Doubly Linked List Queue (Value).
    - Once we retrieve the best Bid or Ask in **O(1) time** from the Tree Structure we can access the orders at this price level in **O(1) time** through the Unordered Map.

- **FIFO Queues - Doubly Linked Lists**
    - Every order in the book is added to the Doubly Linked List held at the price level corresponding to that of the order. When an order arrives we lookup the corresponding FIFO queue using the Unordered map in **O(1) time** and append it to the back to ensure the orders which arrive first are matched first.
    - The Doubly Linked List structure has a pointer to the head which enables access to the highest priority order (first) in **O(1) time**, and new orders can be added to the end of the list in **O(1) time** using the pointer to the tail.

- **Orders***
    - An Order class has been created to hold all of the data associated with an order. An order class allows us to formally structure the attributes of an order, including:
        - Price
        - Type
        - Direction/Side
        - Size
        - ID
        - Time of arrival
        - Next & Prev pointers for the FIFO Queues
    *Not all orders require the same data and some require additional fields so this Order class is a parent from which additional child Order classes can be derived.*
    - As stated a Limit order class has been defined which allows us to create Stop Limit Orders with an additional price field. Once the original target price field is hit by the book, the Limit Order is moved to its target price in the book. 

## Matching Engine
- Limit Order Matching
- Market Order Execution

*Sweeping the book results in slippage*

## Types of Orders Accepts
- Market Orders
- Limit Orders
- Stop Limit Orders
- Fill or Kill Orders

## Visualising the Book
- Shows Cumulative Bid/Ask orders strating from the best Bid/Ask to worst Bid/Ask
    - Cmd Output
    - GNU Plot Chart

## Missing Pieces & Future Improvements
- Thoroughly learn C++ best practices to improve the existing codebase and reduce latency.
- Convert the time when an order is added to the book into "seconds after midnight with decimal precision of at least milliseconds".
- Cutom Red Black Tree (set implementation for now due to the underlying strtucture being a Red black Tree)
- Lock free queue implementation for lower latency
- Multithreading (Must consider thread safety)
- Metrics such as order flow, liquidity ratios, fill rate.
- Update the chart class to have a more appropriate label in the mid point between the best Bid & Ask instead of -1.

