#include <iostream>
#include "orderbook/trees/avl_tree.h"

namespace orderbook
{
    class book
    {
        public:
            orderbook::trees::avl_tree* bid_tree;
            orderbook::trees::avl_tree* ask_tree;
            orderbook::maps::order_map* bid_map;
            orderbook::maps::order_map* ask_map;
            std::int64_t id;
            std::int64_t n_tick_levels;

            book(std::int64_t n)
            {
                id = 0;
                n_tick_levels = n;
                bid_tree = new orderbook::trees::avl_tree{n_tick_levels};
                ask_tree = new orderbook::trees::avl_tree{n_tick_levels};
                bid_map = new orderbook::maps::order_map{n_tick_levels};
                ask_map = new orderbook::maps::order_map{n_tick_levels};
            }

            bool can_match_market_orders(std::int64_t price, bool is_bid_order) {
                bool orders_exist_on_othr_side = is_bid_order ? !ask_tree->is_empty() : !bid_tree->is_empty();
                if(orders_exist_on_othr_side){
                    bool bid_condition = is_bid_order && price >= ask_tree->get_min_value();
                    std::cout << "BID MAX VALUE: " << bid_tree->get_max_value() << "\n";
                    bool ask_condition = !is_bid_order && bid_tree->get_max_value() >= price;
                    return (bid_condition || ask_condition);
                }
                return false;
            }

            void execute_market_order(std::int64_t id, std::int64_t tick_level, std::int64_t order_side, std::int64_t order_size, std::int64_t order_type) {
                // Immediately executed against the best available price in the opposite side of the order book (bids for sell orders, asks for buy orders).
                // Sweep book till order filled or no more orders in book.
                // Slippage can occur when we sweep up or down price levels to fill the order.
                bool order_filled = false;
                std::int64_t initial_size = order_size;
                bool is_bid_order = (order_side == order_side::BID);

                orderbook::trees::avl_tree* target_tree = is_bid_order ? ask_tree : bid_tree;
                orderbook::maps::order_map* target_queue = is_bid_order ? ask_map : bid_map;

                std::cout << "MARKET ORDER: Price: " << tick_level << " Size: " << initial_size << " is bid order: " << is_bid_order << "\n";

                while (!order_filled && can_match_market_orders(tick_level, is_bid_order)) { 
                    // Continue sweeping while price levels exist.
                
                    std::int64_t best_price = is_bid_order ? target_tree->get_min_value() : target_tree->get_max_value(); // Get the best price (min for ask, max for bid).                    
                    orderbook::order* best_order = target_queue->get_priority_order(best_price);

                    std::cout << "CHCKING MARKET ORDER CONDITIONS\n";
                
                    if (handle_stop_limit(best_order, best_price) || handle_fill_or_kill(best_order, best_price, target_queue->get_total_volume_at_tick_level(best_price))) {
                        continue;
                    }

                    if(order_size < best_order->get_size()) {
                        // Market order size is less than the best price level order size (partial fill of best order).
                        target_queue->partial_fill_priority(best_price, order_size);
                        order_filled = true;
                        std::cout << "Market order and best match partial filled, BEST MATCH SIZE: " << best_order->get_size() << " ORDER SIZE: " << order_size << "\n";
                    } else {
                        
                        if (order_size > best_order->get_size()) {
                            // Market order size is greater than the best price level order size (partial fill of market order).
                            // Remove fully filled best order.
                            order_size -= best_order->get_size();
                            std::cout << "Market order partial fill and best match filled, BEST MATCH SIZE: " << best_order->get_size() << " ORDER SIZE: " << order_size << "\n";
                        } else {
                            // Market order size equals the best price level order size (full fill for both).
                            order_filled = true;
                            std::cout << "Market order and best match filled\n";
                        }
                        target_queue->remove_priority_order(best_price);
                    }
                    
                    if(is_bid_order) {
                        remove_empty_ask_level(best_price); // check the best ask is empty after matching, if so remove it
                    } else {
                        remove_empty_bid_level(best_price); // check the best bid is empty after matching, if so remove it
                    };
                }

                if (!order_filled) {
                    if (order_size < initial_size) {
                        std::cout << "Market Order Partial Fill\n"; 
                    } else {
                        std::cout << "Market Order Not Filled - Added To Book as Limit Order\n"; 
                    }
                    add_to_book(tick_level, order_side, order_size, order_type::ORDER_LIMIT, -1, id);
                } else {
                    std::cout << "Market Order Filled\n"; 
                }
            }

            void add_to_book(std::int64_t tick_level, std::int64_t order_side, std::int64_t order_size, std::int64_t order_type, std::int64_t order_limit_price = -1, std::int64_t id_override = -1)
            {
                if(tick_level >= n_tick_levels)
                {
                    std::cout << "TICK LEVEL IS OUT OF BOUNDS OF AVAILABLE LEVELS.\n";
                    std::cout << "-> dropping order.\n";
                    return;
                }
                std::int64_t order_id = (id_override == -1) ? id++ : id_override;

                print();
                if(order_type == order_type::ORDER_MARKET) { // market
                    execute_market_order(order_id, tick_level, order_side, order_size, order_type);
                } else {
                    if(order_side == 1) {
                        // bid
                        bid_tree->insert(tick_level);
                        bid_map->add_order(order_id, tick_level, order_side, order_size, order_type, order_limit_price);
                    } else {
                        // ask
                        ask_tree->insert(tick_level);
                        ask_map->add_order(order_id, tick_level, order_side, order_size, order_type, order_limit_price);
                    }
                }
                print();
            }

            bool bids_and_asks_exist() {
                return (!bid_tree->is_empty() && !ask_tree->is_empty());
            }

            bool can_match_orders() {
                // Ensure the max bid is greater than the min ask.
                return (bids_and_asks_exist() && bid_tree->get_max_value() >= ask_tree->get_min_value());
            }

            std::int64_t get_resting_order_execution_price(std::int64_t bid_id, std::int64_t bid_price, std::int64_t ask_id, std::int64_t ask_price) {
                return (bid_id < ask_id) ? bid_price : ask_price;
            }

            void remove_empty_tick_levels(std::int64_t best_bid_price, std::int64_t best_ask_price) {
                if (bid_map->is_empty(best_bid_price)) {
                    bid_tree->remove(best_bid_price);
                }
                if (ask_map->is_empty(best_ask_price)) {
                    ask_tree->remove(best_ask_price);
                }
            }

            void remove_empty_bid_level(std::int64_t bid_level) {
                if (bid_map->is_empty(bid_level)) bid_tree->remove(bid_level);
            }

            void remove_empty_ask_level(std::int64_t ask_level) {
                if (ask_map->is_empty(ask_level)) ask_tree->remove(ask_level);
            }

            bool handle_fill_or_kill(orderbook::order* order, std::int64_t price_level, int total_volume_available) {
                if (order->get_type() == 3 && total_volume_available < order->get_size()) {
                    // Order is bigger than total volume at best price level on other side.
                    std::cout << "Fill or kill Order: " << order->get_order_id() << " Cancelled, Reason: Insufficient Volume\n";
                    if(order->get_side() == 1) {
                        // bid
                        bid_map->remove_priority_order(price_level);
                        remove_empty_bid_level(price_level);
                    } else {
                        // ask
                        ask_map->remove_priority_order(price_level);
                        remove_empty_ask_level(price_level);
                    }
                    return true;
                }
                return false;
            }

            bool handle_stop_limit(orderbook::order* order, std::int64_t price_level) {
                if (order->get_type() == 2) { // stop limit order detected, move to specified limit price level on book.
                    std::cout << "Limit order Hit, ID: " << order->get_order_id() << "\n";
                    if(order->get_side() == 1) {
                        // bid
                        bid_map->remove_priority_order(price_level);
                        remove_empty_bid_level(price_level);
                    } else {
                        // ask
                        ask_map->remove_priority_order(price_level);
                        remove_empty_ask_level(price_level);
                    }
                    // Add limit order at limit price to book when stop hit.
                    add_to_book(order->get_limit_price(), order->get_side(), order->get_size(), 1, -1, order->get_order_id());
                    return true;
                }
                return false;
            }

            void print()
            {
                std::cout << "ORDERBOOK\n";
                std::cout << "--------------------------------\n";
                bool found_market_price = false;
                for(std::int64_t tick_level = n_tick_levels-1; tick_level > -1; tick_level--) {
                    std::int64_t ask_vol = ask_map->get_total_volume_at_tick_level(tick_level);
                    std::int64_t bid_vol = bid_map->get_total_volume_at_tick_level(tick_level);
                    if(!found_market_price && bid_vol) {
                        std::cout << "=>\n";
                        found_market_price = true;
                    }
                    std::string ask_volume(ask_vol, 'A');
                    std::string bid_volume(bid_vol, 'B');
                    std::cout << tick_level << " | " << bid_volume << ask_volume << "\n";
                }
                if(!found_market_price) {
                    std::cout << "-<\n";
                }
                std::cout << "--------------------------------\n";
            }

            void match_orders()
            {
                while (can_match_orders()) {

                    print();

                    std::int64_t best_bid_price = bid_tree->get_max_value();
                    std::int64_t best_ask_price = ask_tree->get_min_value();
                    orderbook::order* bid = bid_map->get_priority_order(best_bid_price);
                    orderbook::order* ask = ask_map->get_priority_order(best_ask_price);
                    std::int64_t bid_id = bid->get_order_id();
                    std::int64_t ask_id = ask->get_order_id();
                    std::int64_t execution_price = get_resting_order_execution_price(bid_id, best_bid_price, ask_id, best_ask_price);
  
                    if (handle_stop_limit(bid, best_bid_price) || handle_stop_limit(ask, best_ask_price)) continue;
                    if (handle_fill_or_kill(bid, best_ask_price, ask_map->get_total_volume_at_tick_level(best_ask_price))) continue;
                    if (handle_fill_or_kill(ask, best_bid_price, bid_map->get_total_volume_at_tick_level(best_bid_price))) continue;

                    if (bid->get_size() == ask->get_size()) {
                        // Both Bid and Ask match in size and can be removed.
                        bid_map->remove_priority_order(best_bid_price);
                        ask_map->remove_priority_order(best_ask_price);
                        std::cout << "Bid Order: " << bid_id << " matched with Ask Order: " << ask_id << " at Price: " << execution_price << "\n";
                    } else if (bid->get_size() > ask->get_size()) {
                        // Ask filled fully, Bid partial fill.
                        bid_map->partial_fill_priority(best_bid_price, ask->get_size()); // <<< NEEDS TO BE HANDLED
                        ask_map->remove_priority_order(best_ask_price); 
                        std::cout << "Bid Order Partial Fill: " << bid_id << " matched with Ask Order: " << ask_id << " at Price: " << execution_price << "\n";
                    } else {
                        // Bid filled fully, Ask partial fill.
                        ask_map->partial_fill_priority(best_ask_price, bid->get_size()); // <<< NEEDS TO BE HANDLED
                        bid_map->remove_priority_order(best_bid_price);
                        std::cout << "Bid Order: " << bid_id << " matched with Ask Order Partial Fill: " << ask_id << " at Price: " << execution_price << "\n";
                    }
                    remove_empty_tick_levels(best_bid_price, best_ask_price);

                    print();
                }
                std::cout << "* Finished Matching *" << "\n";
            }

            ~book()
            {
                delete bid_tree;
                delete ask_tree;
                delete bid_map;
                delete ask_map;
            }
    };
}
