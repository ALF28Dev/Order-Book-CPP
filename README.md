# C++ Based Order Book


> [!IMPORTANT]
> ### What's new in Version 2.0?
> - Memory Pools & Eager Allocation
> - Custom Data Structures


Hey everyone, thank you for taking the time to view this repository. This is my C++ based order book project, which I used as an opportunity to learn some basic C++ and fundamental concepts related to market microstructure. The project aims to replicate core functionalities of one of the cornerstone components of an exchange: the order book. I used the included [LOBSTER](https://lobsterdata.com/) readme.txt file to learn about what information is associated with every order in an order book on an exchange.

<div align="center">
    <img width="100%" height="701" alt="Screenshot 2025-09-27 at 21 44 00" src="https://github.com/user-attachments/assets/701cde77-6e2a-4c34-9905-5a5caa993942" /><br>
    <figcaption>Figure 1: Orderbook Structure and Visual</figcaption>
</div>




## Matching Engine
- **Limit Order Matching**
    - **Order Matching Conditions**
        - The Limit order matching system is implemented within the function `match_orders()`. This function matches Orders within the book as long as the book satisfies the conditions for matching orders. This condition requires orders to exist on both sides of the book & the maximum bid (Max Bid) is greater than or equal to the minimum ask (Min Ask). This means someone is willing to pay more than the lowest price that someone is willing to pay; therefore, we can perform a match.
    - **Fill Types (Full and Partial Fill)**
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






## Sweeping & Slippage

## Available Order Types
- **Market Orders**
    - Market orders immediately get executed at the best price level on the other side of the book.
- **Limit Orders**
    - Limit orders get added to the book at the specified price level and remain in the book until they are executed.
- **Stop Limit Orders**
    - Stop Limit orders get added to the book at a specified price level with an additional target price level. When the specified price level is reached by the book the order is moved and added to the book at the target price level. It remains in this price level until it gets executed.
- **Fill or Kill Orders**
    - Fill or Kill orders are limit orders which are added to the book at a specified price. When the price level is reached and the order is ready to be matched we first check to ensure the best price level on the other side of the book has sufficient volume to fill the entire order, otherwise we remove it from the book.


## Custom Data Structures



### The Ring Buffer

<div align="center">
    <img width="80%" height="526" alt="Order-Book-CPP-Ring-Buffer" src="https://github.com/user-attachments/assets/11ad8349-1f63-4f08-a16b-d7ca284a9cc5" /><br>
    <figcaption>Figure 2: Ring Buffer for Order Ingress</figcaption>
</div>


### The Order Map

<div align="center">
    <img width="80%" height="844" alt="Order-Book-CPP-Order-Map" src="https://github.com/user-attachments/assets/0235933f-6f26-4cc9-8d53-72a4a1e45da4" /><br>
    <figcaption>Figure 3: Order Map for Holding Ring Buffers for Each Tick Level</figcaption>
</div>

### The AVL Tree

<div align="center">
    <img width="80%" height="689" alt="Order-Book-CPP-AVL-Tree" src="https://github.com/user-attachments/assets/92a3c36c-df0f-47d8-af80-63cfb11d1a1b" /><br>
    <figcaption>Figure 4: AVL Tree for Efficiently Structuring Tick Levels for Bids/Asks</figcaption>
</div>

### The Tick Level Bitmap

<div align="center">
    <img width="80%" height="391" alt="Screenshot 2025-09-27 at 15 12 52" src="https://github.com/user-attachments/assets/23d0f982-b7b7-4a88-97c4-14436e1b7b2a" /><br>
    <figcaption>Figure 5: Tick Level Bitmap for Efficiently Confirming if a Tick Level has orders Pending Ingress</figcaption>
</div>

### The Memory Pool Bitmap

<div align="center">
    <img width="80%" height="543" alt="Screenshot 2025-09-27 at 15 18 34" src="https://github.com/user-attachments/assets/931ea460-ab62-4484-b751-ffc10b5de0d5" /><br>
    <figcaption>Figure 6: Memory Pool Bitmap for Efficiently tracking used/free memory slots in Allocated Memory Pools</figcaption>
</div>



## Project Vision: What’s Next?

- Lock-Free Data Structures from the Boost Library.
- Metrics such as order flow, liquidity ratios, and fill rate.
- Order input via sockets.
- Log outputs via sockets for processing. 
- Implement Chart Visualisation in version 2.0 using logic from version 1.0.





