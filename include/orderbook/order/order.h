#include "orderbook/enums/enums.h"

namespace orderbook 
{
    class order
    {
        public:
            std::int64_t order_size;
            std::int64_t order_side;
            std::int64_t order_id;
            std::int64_t order_type;
            std::int64_t order_limit_price;

            order(std::int64_t order_side, std::int64_t order_size) : 
                order_side(order_side), order_size(order_size), order_id(-1), order_type(-1), order_limit_price(-1) {};

            void set_market_order_attributes(std::int64_t id, std::int64_t size, std::int64_t side)
            {
                order_id = id;
                order_size = size;
                order_side = side;
                order_type = order_type::ORDER_MARKET; // limit/market
            }
            
            void set_limit_order_attributes(std::int64_t id, std::int64_t size, std::int64_t side, std::int64_t limit_price)
            {
                order_id = id;
                order_size = size;
                order_side = side;
                order_limit_price = limit_price;
                order_type = order_type::ORDER_STOP_LIMIT; // stop limit order
            }

            void reduce_size(std::int64_t size_of_match)
            {
                order_size -= size_of_match;
            }

            std::int64_t get_size()
            {
                return order_size;
            }

            std::int64_t get_type()
            {
                return order_type;
            }

            std::int64_t get_side()
            {
                return order_side;
            }

            std::int64_t get_order_id()
            {
                return order_id;
            }

            std::int64_t get_limit_price() {
                return order_limit_price;
            }
            
            ~order()
            {
                std::cout << "deleting order.\n";
            }
    };
}
