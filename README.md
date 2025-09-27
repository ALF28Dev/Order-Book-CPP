# C++ Based Order Book

Hey everyone, thank you for taking the time to view this repository. This is my C++ based order book project, which I used as an opportunity to learn some basic C++ and fundamental concepts related to market microstructure. The project aims to replicate core functionalities of one of the cornerstone components of an exchange: the order book. I used the included [LOBSTER](https://lobsterdata.com/) readme.txt file to learn about what information is associated with every order in an order book on an exchange.

<div align="center">
<img width="100%" height="701" alt="Screenshot 2025-09-27 at 21 44 00" src="https://github.com/user-attachments/assets/701cde77-6e2a-4c34-9905-5a5caa993942" /><br>
    <figcaption>Figure 1: Orderbook Structure and Visual</figcaption>

</div>


## What's new in Version 2.0?
#### Memory Pools & Eager Allocation


###


## The Custom Data Structures



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


