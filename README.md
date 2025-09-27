> [!WARNING]
> README in progress.

# C++ Based Order Book

> [!IMPORTANT]
> ### What's new in Version v2.0.0?
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
        - The Limit order matching system is implemented within the function `match_orders()`. This function matches Orders within the book as long as the book satisfies the conditions for matching orders. This condition requires orders to exist on both sides of the book & the maximum bid (Max Bid) is greater than or equal to the minimum ask (Min Ask). This means that someone is willing to pay more or equal to the lowest ask (Min Ask); therefore, we can perform a match.
    - **Handling Fill Types**
        - The book structure I have created supports both full and partial fills.
        - If the size of the order on one side of the book is less than the order on the other side with which it is being matched, the smaller order will be filled. In contrast, the larger order will be partially filled, with the remaining unmatched quantity staying in the order book.
        - **Partial Fills**
            - When an order is partially filled, it remains in the book in the same position within the FIFO Ring Buffer Queue, except its size is updated to reflect how much of the order has been filled.
        - **Full Fills**
            - When an order is filled, it is removed from the book. It is removed from the FIFO queue, and a new order takes its place as the highest priority.
            - If no other orders exist at that price level, the price level is removed from the AVL Tree to ensure the Min & Max functions always retrieve the best price levels containing orders.
    - **Handling Stop Limit Orders**
        - During order matching, we always fetch the best Bid & Ask when the conditions for matching are met. If either of these orders is a Stop-Limit order, we first verify that it has been moved to its correct position in the book. If it has not, we update the price field of the order to reflect the target price set by the user, and move the order to the correct position in the book.
    - **Handling Fill-Or-Kill Orders**
        - If either of the best Bid & Ask orders is a Fill-Or-Kill order, we perform an additional check which ensures that the best price level on the other side of the book contains sufficient available volume to fill the order. If not, we remove the order from the book and do not perform a match.
- **Execution Price**
  - When a match occurs, the resting order execution price is used as the execution price. This dictates that the execution price between two matched orders should be the price of the order which was first in the book.

## Sweeping & Slippage

## Available Order Types
- **Market Orders**
    - Market orders immediately get executed at the best price level on the other side of the book.
- **Limit Orders**
    - Limit orders get added to the book at the specified price level and remain in the book until they are executed.
- **Stop Limit Orders**
    - Stop Limit orders get added to the book at a specified price level with an additional target price level. When the book reaches the specified price level, the order is moved and added to the book at the target price level. It remains at this price level until it gets executed.
- **Fill or Kill Orders**
    - Fill or Kill orders are limit orders which are added to the book at a specified price. When the price level is reached and the order is ready to be matched, we first verify that the best price level on the other side of the book has sufficient volume to fill the entire order; otherwise, we remove it from the book.


## Custom Data Structures

### The Ring Buffer - Order Ingress

A fixed-size Ring Buffer data structure has been implemented to handle order ingress in a FIFO manner. Tail pointers track the first order ingressed, and head pointers point to the next order object within the ring buffer (this may overwrite existing orders, given we allocate a block of memory at process startup and continuously reuse order objects). As we match orders, we increment the tail, and as we ingress orders, we increment the head. Head/Tail pointers are used as offsets from the first address within the memory pool. 

<div align="center">
    <img width="100%" height="526" alt="Order-Book-CPP-Ring-Buffer" src="https://github.com/user-attachments/assets/11ad8349-1f63-4f08-a16b-d7ca284a9cc5" /><br>
</div><br>

>[!NOTE]
> - **Eager Allocation:** This Ring Buffer data structure has been implemented using a fixed-size memory pool. At process startup, a pool of memory is allocated for X number of Order objects within the Ring Buffer, and each slot is eagerly allocated with an Order object, removing the need to allocate memory for `new` Order objects during process execution.
> - **Cache Locality:** Allocating a fixed-size memory pool provides the added benefit of cache locality. As the head and tail indices advance while orders are ingested and matched, we move across adjacent cache lines, resulting in fewer cache misses, a higher TLB hit rate, and fewer page faults.



### The Order Map - Tick Level to Ring Buffer Map

A fixed-size memory pool is allocated at startup for X number of slots, each representing a tick level within the order book. Within each slot, we eagerly allocate a Ring Buffer structure to manage order ingress at each tick level within the order book. When the Max/Min Bid/Ask is identified within the corresponding AVL tree, we can quickly look up the order ingress queue related to that tick level by using the tick level value as an offset from the first address within the memory pool allocated for the order map.

<div align="center">
    <img width="80%" height="844" alt="Order-Book-CPP-Order-Map" src="https://github.com/user-attachments/assets/0235933f-6f26-4cc9-8d53-72a4a1e45da4" /><br>
    <figcaption>Figure 3: Order Map for Holding Ring Buffers for Each Tick Level</figcaption>
</div>

>[!NOTE]
> - **Eager Allocation:** The eager allocation of a Ring Buffer within each tick level slot of the order map removed the need to allocate `new` ingress ring buffers as we match orders and move across tick levels. As orders are ingressed at each level, we don't need to allocate any `new` Order objects, as previously stated in the section above, as we continuously reuse the allocated order objects. Allocating a Ring buffer at each tick level at process startup removes the need to create `new` and `delete` old Ring Buffers as price levels are occupied and dropped during matching.

### The AVL Tree - Tick Level Structure

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
- Testing using Valgrind/AddressSanitizer.
- Lock-Free Data Structures from the Boost Library.
- Metrics such as order flow, liquidity ratios, and fill rate.
- Order input via sockets.
- Log outputs via sockets for processing. 
- Implement Chart Visualisation in version 2.0 using logic from version 1.0.
- Good Until Cancelled Order Type.
- Ensure objects allocated within memory pools are correctly aligned in memory.
- Move away from AVL tree-based implementation to a bitmap-based approach, where we move between price levels by finding the next or previous free level by finding the next/previous set/unset bit.





