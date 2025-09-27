#include "orderbook/order/order.h"

namespace orderbook::queues {
    class ring_buffer
    {
        private:
            orderbook::order* mempool;
            std::int64_t mempool_size;
            std::int64_t head;
            std::int64_t tail;
            std::int64_t total_volume;

        public:
            ring_buffer(std::int64_t ms)
            {
                head = 0;
                tail = 0;
                total_volume = 0;
                mempool_size = ms;
                mempool = static_cast<orderbook::order*>(std::malloc(mempool_size * sizeof(orderbook::order)));
                for(std::size_t i = 0; i < mempool_size; i++)
                {
                    std::cout << "allocating order at index: " << i << "\n";
                    new (mempool+i) orderbook::order{-2, 0};
                };
            }

            ~ring_buffer()
            {
                for(std::size_t i = 0; i < mempool_size; i++)
                {
                    std::cout << "freeing index: " << i << "\n";
                    (mempool+i)->~order();
                };
                std::free(mempool);
                std::cout << "freeing memory of ring_buffer.\n";
            }

            void enqueue(std::int64_t id, std::int64_t order_side, std::int64_t order_size, std::int64_t order_type, std::int64_t order_limit_price)
            {
                std::cout << "order enequeue, size: " << order_size << "\n";
            
                total_volume -= (mempool+(head%mempool_size))->get_size(); // handle wrap around.            
                if((mempool+(head%mempool_size))->get_side() != -2) {
                    // ring buffer full drop orders.
                    std::cout << "OVERWRITING RING BUFFER, DROPPING EXISTING ORDER\n";
                }

                if(order_type == order_type::ORDER_MARKET || order_type == order_type::ORDER_LIMIT){
                    // market and limit orders
                    (mempool+(head%mempool_size))->set_market_order_attributes(id, order_size, order_side);
                } else {
                    // stop limit orders
                    (mempool+(head%mempool_size))->set_limit_order_attributes(id, order_size, order_side, order_limit_price);
                }
                total_volume += order_size;
                head++;
            }

            bool is_empty()
            {
                return tail == head;
            }

            orderbook::order* dequeue()
            {
                if(tail == head)
                {
                    std::cout << "NO ORDERS IN QUEUE!\n";
                    return nullptr;
                }
                orderbook::order* tmp = (mempool+(tail%mempool_size));
                tail++;
                if(tail > head)
                {
                    head++;
                }
                total_volume -= tmp->get_size();
                tmp->order_side = -1;
                return tmp;
            }

            std::int64_t get_total_volume()
            {
                return total_volume;
            }

            orderbook::order* peek()
            {
                if(tail == head)
                {
                    std::cout << "NO ORDERS IN QUEUE!\n";
                    return nullptr;
                }
                return (mempool+(tail%mempool_size));
            }

            void reduce_size_of_tail(std::int64_t size_of_match)
            {
                total_volume -= size_of_match;
                (mempool+(tail%mempool_size))->reduce_size(size_of_match);
            }
    };
}
