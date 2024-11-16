# Order-Book-CPP
![GNU Plot of Order book](./example-orderbook-chart.png)

Hey everyone, thank you for taking the time to view this repository. This is my C++ based order book project, which I used as an opportunity to learn some basic C++ and fundamental concepts related to market microstructure. The project aims to replicate core functionalities of one of the cornerstone components of an exchange: the order book. I used the included [LOBSTER](https://lobsterdata.com/) readme txt file to learn about what information is associated with every order in an order book on an exchange.

## Book Structure
- Set for the red black tree

## Matching Engine
- Limit Order Matching
- Market Order Execution

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


