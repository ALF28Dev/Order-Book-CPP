#include <iostream>
#include "orderbook/queues/ring_buffer.h"

namespace orderbook::maps
{
    class order_map
    {
        private:
            orderbook::queues::ring_buffer* mempool;
            std::int64_t mempool_size;

        public:

            order_map(std::int64_t ms)
            {
                mempool_size = ms;
                mempool = static_cast<orderbook::queues::ring_buffer*>(std::malloc(mempool_size * sizeof(orderbook::queues::ring_buffer)));
                for(std::int64_t i = 0; i < mempool_size; i++)
                {
                    new (mempool+i) orderbook::queues::ring_buffer{10}; // Only 10 orders can be ingressed currently.
                };
            }

            void add_order(std::int64_t id, std::int64_t tick_level, std::int64_t order_side, std::int64_t order_size, std::int64_t order_type, std::int64_t order_limit_price)
            {
                std::cout << "inserting " << ((order_side == 1) ? "bid" : "ask") << " order in queue at tick_level: " << tick_level << "\n";
                (mempool+tick_level)->enqueue(id, order_side, order_size, order_type, order_limit_price);
            }

            orderbook::order* remove_priority_order(std::int64_t tick_level)
            {
                return (mempool+tick_level)->dequeue();
            }

            orderbook::order* get_priority_order(std::int64_t tick_level)
            {
                return (mempool+tick_level)->peek();
            }

            void partial_fill_priority(std::int64_t tick_level, std::int64_t size_of_match) {
                (mempool+tick_level)->reduce_size_of_tail(size_of_match);
            }

            bool is_empty(std::int64_t tick_level)
            {
                return (mempool+tick_level)->is_empty();
            }

            ~order_map()
            {
                for(std::int64_t i = 0; i < mempool_size; i++)
                {
                    (mempool+i)->~ring_buffer();
                };
                std::free(mempool);
            }

            std::int64_t get_total_volume_at_tick_level(std::int64_t tick_level)
            {
                return (mempool+tick_level)->get_total_volume();
            }

            void print_order_map(std::int64_t side)
            {
                for (std::int64_t k = 0; k < mempool_size; k++) {
                    // print asks highest to lowest
                    // print bids lowest to highest
                    std::int64_t i = (side == 1) ? k : (mempool_size - 1 - k);
                    std::int64_t tick_level_total_volume = (mempool + i)->get_total_volume();
                    std::string output(tick_level_total_volume, '#');
                    std::cout << i << " | " << output << "\n";
                }
            }
    };
}
