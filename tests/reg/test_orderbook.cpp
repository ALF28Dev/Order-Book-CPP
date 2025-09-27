#include <gtest/gtest.h>
#include <orderbook/orderbook/orderbook.h>

TEST(test_orderbook, test_bids_and_asks_exist_one) {
    orderbook::book* ob = new orderbook::book{10};
    ob->bids_and_asks_exist();
    EXPECT_EQ(ob->bids_and_asks_exist(), false);
};

TEST(test_orderbook, test_bids_and_asks_exist_two) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 1, order_type::ORDER_LIMIT);
    ob->bids_and_asks_exist();
    EXPECT_EQ(ob->bids_and_asks_exist(), false);
};

TEST(test_orderbook, test_bids_and_asks_exist_three) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 1, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::BID, 1, order_type::ORDER_LIMIT);
    ob->bids_and_asks_exist();
    EXPECT_EQ(ob->bids_and_asks_exist(), true);
};

TEST(test_orderbook, test_can_match_orders_one) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 1, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::BID, 1, order_type::ORDER_LIMIT);
    EXPECT_EQ(ob->can_match_orders(), true);
};

TEST(test_orderbook, test_can_match_orders_two) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 1, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::BID, 1, order_type::ORDER_LIMIT);
    EXPECT_EQ(ob->can_match_orders(), true);
};

TEST(test_orderbook, test_can_match_orders_three) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(6, order_side::ASK, 1, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::BID, 1, order_type::ORDER_LIMIT);
    EXPECT_EQ(ob->can_match_orders(), false);
};

TEST(test_orderbook, test_can_match_orders_four) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 1, order_type::ORDER_LIMIT);
    EXPECT_EQ(ob->can_match_orders(), false);
};

TEST(test_orderbook, test_can_match_orders_five) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::BID, 1, order_type::ORDER_LIMIT);
    EXPECT_EQ(ob->can_match_orders(), false);
};

TEST(test_orderbook, test_can_match_orders_six) {
    orderbook::book* ob = new orderbook::book{10};
    EXPECT_EQ(ob->can_match_orders(), false);
};

TEST(test_orderbook, test_match_orders) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->match_orders();
    EXPECT_EQ(ob->can_match_orders(), false);
};

TEST(test_orderbook, test_orderbook_ask_map_one) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 99, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 88, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 77, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->ask_map->remove_priority_order(5);
    EXPECT_EQ(ob->ask_map->get_priority_order(5)->get_size(), 88);
};

TEST(test_orderbook, test_orderbook_ask_map_two) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 99, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 88, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 77, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->ask_map->remove_priority_order(5);
    ob->ask_map->remove_priority_order(5);
    EXPECT_EQ(ob->ask_map->is_empty(5), false);
};

TEST(test_orderbook, test_orderbook_ask_map_three) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 99, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 88, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 77, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->ask_map->remove_priority_order(5);
    ob->ask_map->remove_priority_order(5);
    ob->ask_map->remove_priority_order(5);
    EXPECT_EQ(ob->ask_map->is_empty(5), true);
};

TEST(test_orderbook, test_orderbook_ask_map_four) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 99, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 88, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 77, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->ask_map->remove_priority_order(5);
    ob->ask_map->remove_priority_order(5);
    ob->ask_map->partial_fill_priority(5, 7);
    EXPECT_EQ(ob->ask_map->get_priority_order(5)->get_size(), 70);
};

TEST(test_orderbook, test_orderbook_ask_map_five) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 99, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 88, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 77, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->ask_map->remove_priority_order(5);
    ob->ask_map->remove_priority_order(5);
    ob->ask_map->partial_fill_priority(5, 7);
    ob->ask_map->partial_fill_priority(5, 10);
    ob->ask_map->partial_fill_priority(5, 5);
    ob->ask_map->partial_fill_priority(5, 5);
    EXPECT_EQ(ob->ask_map->get_priority_order(5)->get_size(), 50);
};

TEST(test_orderbook, test_match_orders_two) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->match_orders();
    EXPECT_EQ(ob->ask_map->is_empty(5), true);
};

TEST(test_orderbook, test_match_orders_three) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->match_orders();
    EXPECT_EQ(ob->ask_map->is_empty(6), true);
};

TEST(test_orderbook, test_match_orders_four) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);\
    ob->add_to_book(6, order_side::BID, 10, order_type::ORDER_LIMIT);
    ob->match_orders();
    EXPECT_EQ(ob->bid_map->is_empty(6), false);
};

TEST(test_orderbook, test_match_orders_five) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);\
    ob->add_to_book(6, order_side::BID, 10, order_type::ORDER_LIMIT);
    ob->match_orders();
    EXPECT_EQ(ob->ask_map->is_empty(5), true);
};

TEST(test_orderbook, test_match_orders_six) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(5, order_side::ASK, 2, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 3, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 10, order_type::ORDER_LIMIT);
    ob->match_orders();
    EXPECT_EQ(ob->ask_map->is_empty(5), true);
};

TEST(test_orderbook, test_match_orders_seven) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(6, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 2, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 3, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 10, order_type::ORDER_LIMIT);
    ob->match_orders();
    EXPECT_EQ(ob->ask_map->is_empty(5), true);
    EXPECT_EQ(ob->ask_map->is_empty(6), true);
    EXPECT_EQ(ob->bid_map->is_empty(6), false);
};

TEST(test_orderbook, test_match_orders_eight) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(6, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 2, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 3, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 11, order_type::ORDER_LIMIT);
    ob->match_orders();
    EXPECT_EQ(ob->ask_map->is_empty(5), true);
    EXPECT_EQ(ob->ask_map->is_empty(6), true);
    EXPECT_EQ(ob->bid_map->get_total_volume_at_tick_level(6), 6);
};

TEST(test_orderbook, test_match_orders_nine) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(6, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 2, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 3, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 11, order_type::ORDER_LIMIT);
    ob->match_orders();
    EXPECT_EQ(ob->ask_map->is_empty(5), true);
    EXPECT_EQ(ob->ask_map->is_empty(6), true);
    EXPECT_EQ(ob->ask_map->get_total_volume_at_tick_level(6), 0);
};

TEST(test_orderbook, test_match_orders_ten) {
    orderbook::book* ob = new orderbook::book{10};
    ob->add_to_book(6, order_side::ASK, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 2, order_type::ORDER_LIMIT);
    ob->add_to_book(5, order_side::ASK, 3, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 5, order_type::ORDER_LIMIT);
    ob->add_to_book(6, order_side::BID, 11, order_type::ORDER_LIMIT);
    ob->match_orders();
    EXPECT_EQ(ob->ask_map->is_empty(5), true);
    EXPECT_EQ(ob->ask_map->is_empty(6), true);
    EXPECT_EQ(ob->ask_map->get_total_volume_at_tick_level(5), 0);
};