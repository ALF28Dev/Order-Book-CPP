#include <gtest/gtest.h>
#include <orderbook/maps/order_map.h>

TEST(order_map_test, test_enqueue_dequeue) {
    orderbook::maps::order_map* order_map = new orderbook::maps::order_map{10};
    order_map->add_order(1, 5, 1, 88, 1, -1);
    order_map->add_order(2, 5, 1, 99, 1, -1);
    order_map->add_order(3, 4, -1, 77, 1, -1);
    order_map->add_order(4, 4, -1, 55, 1, -1);
    EXPECT_EQ(order_map->remove_priority_order(4)->get_size(), 77);
};

TEST(order_map_test, test_get_priority_order) {
    orderbook::maps::order_map* order_map = new orderbook::maps::order_map{10};
    order_map->add_order(1, 5, 1, 88, 1, -1);
    order_map->add_order(2, 5, 1, 99, 1, -1);
    order_map->add_order(3, 4, -1, 77, 1, -1);
    order_map->add_order(4, 4, -1, 55, 1, -1);
    EXPECT_EQ(order_map->get_priority_order(5)->get_size(), 88);
};

TEST(order_map_test, test_get_partial_fill_priority) {
    orderbook::maps::order_map* order_map = new orderbook::maps::order_map{10};
    order_map->add_order(1, 5, 1, 88, 1, -1);
    order_map->add_order(2, 5, 1, 99, 1, -1);
    order_map->add_order(3, 4, -1, 77, 1, -1);
    order_map->add_order(4, 4, -1, 55, 1, -1);
    order_map->partial_fill_priority(5, 87);
    EXPECT_EQ(order_map->get_priority_order(5)->get_size(), 1);
};

TEST(order_map_test, test_is_emtpy_one) {
    orderbook::maps::order_map* order_map = new orderbook::maps::order_map{10};
    order_map->add_order(1, 5, 1, 88, 1, -1);
    order_map->add_order(2, 5, 1, 99, 1, -1);
    EXPECT_EQ(order_map->is_empty(5), false);
};

TEST(order_map_test, test_is_emtpy_two) {
    orderbook::maps::order_map* order_map = new orderbook::maps::order_map{10};
    order_map->add_order(1, 5, 1, 88, 1, -1);
    order_map->add_order(2, 5, 1, 99, 1, -1);
    EXPECT_EQ(order_map->is_empty(4), true);
};

TEST(order_map_test, test_get_total_volume_at_tick_level_one) {
    orderbook::maps::order_map* order_map = new orderbook::maps::order_map{10};
    order_map->add_order(1, 5, 1, 100, 1, -1);
    order_map->add_order(2, 5, 1, 100, 1, -1);
    order_map->add_order(3, 4, -1, 77, 1, -1);
    order_map->add_order(4, 4, -1, 55, 1, -1);
    EXPECT_EQ(order_map->get_total_volume_at_tick_level(5), 200);
};

TEST(order_map_test, test_get_total_volume_at_tick_level_two) {
    orderbook::maps::order_map* order_map = new orderbook::maps::order_map{10};
    order_map->add_order(1, 5, 1, 100, 1, -1);
    order_map->add_order(2, 5, 1, 100, 1, -1);
    order_map->add_order(3, 4, -1, 77, 1, -1);
    order_map->add_order(4, 4, -1, 55, 1, -1);
    order_map->partial_fill_priority(5, 50);
    EXPECT_EQ(order_map->get_total_volume_at_tick_level(5), 150);
};