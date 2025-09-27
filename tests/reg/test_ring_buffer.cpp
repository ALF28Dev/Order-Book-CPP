#include <gtest/gtest.h>
#include <orderbook/queues/ring_buffer.h>

/**/
TEST(ring_buffer_test, test_enqueue_dequeue) {
    orderbook::queues::ring_buffer* ring_buffer = new orderbook::queues::ring_buffer{4};
    std::int64_t id = 1;
    std::int64_t side = 1;
    std::int64_t size = 10;
    std::int64_t type = 1;
    std::int64_t limit = 99;
    ring_buffer->enqueue(id, side, size, type, limit);
    orderbook::order* order = ring_buffer->dequeue();
    EXPECT_EQ(order->get_order_id(), 1);
};

TEST(ring_buffer_test, test_peek) {
    orderbook::queues::ring_buffer* ring_buffer = new orderbook::queues::ring_buffer{4};
    ring_buffer->enqueue(2, 1, 99, 1, 88);
    ring_buffer->enqueue(3, 1, 99, 1, 88);
    ring_buffer->enqueue(4, 1, 99, 1, 88);
    orderbook::order* order_one = ring_buffer->peek();
    EXPECT_EQ(order_one->get_order_id(), 2);
    orderbook::order* order_two = ring_buffer->peek();
    EXPECT_EQ(order_two->get_order_id(), 2);
};

TEST(ring_buffer_test, test_is_empty_one) {
    orderbook::queues::ring_buffer* ring_buffer = new orderbook::queues::ring_buffer{4};
    bool is_empty_one = ring_buffer->is_empty();
    EXPECT_EQ(is_empty_one, true);
    ring_buffer->enqueue(2, 1, 99, 1, 88);
    bool is_empty_two = ring_buffer->is_empty();
    EXPECT_EQ(is_empty_two, false);
};

TEST(ring_buffer_test, test_is_empty_two) {
    orderbook::queues::ring_buffer* ring_buffer = new orderbook::queues::ring_buffer{4};
    bool is_empty_one = ring_buffer->is_empty();
    EXPECT_EQ(is_empty_one, true);
    ring_buffer->enqueue(2, 1, 99, 1, 88);
    ring_buffer->dequeue();
    bool is_empty_two = ring_buffer->is_empty();
    EXPECT_EQ(is_empty_two, true);
};

TEST(ring_buffer_test, test_get_total_volume_one) {
    orderbook::queues::ring_buffer* ring_buffer = new orderbook::queues::ring_buffer{4};
    ring_buffer->enqueue(2, 1, 99, 1, 88);
    ring_buffer->enqueue(2, 1, 2, 1, 88);
    std::int64_t total_volume = ring_buffer->get_total_volume();
    EXPECT_EQ(total_volume, 101);
};

TEST(ring_buffer_test, test_get_total_volume_two) {
    orderbook::queues::ring_buffer* ring_buffer = new orderbook::queues::ring_buffer{4};
    ring_buffer->enqueue(2, 1, 99, 1, 88);
    ring_buffer->enqueue(2, 1, 2, 1, 88);
    ring_buffer->dequeue();
    std::int64_t total_volume = ring_buffer->get_total_volume();
    EXPECT_EQ(total_volume, 2);
};

TEST(ring_buffer_test, test_get_total_volume_three) {
    orderbook::queues::ring_buffer* ring_buffer = new orderbook::queues::ring_buffer{4};
    ring_buffer->enqueue(2, 1, 99, 1, 88);
    ring_buffer->enqueue(2, 1, 2, 1, 88);
    ring_buffer->dequeue();
    ring_buffer->dequeue();
    std::int64_t total_volume = ring_buffer->get_total_volume();
    EXPECT_EQ(total_volume, 0);
};

TEST(ring_buffer_test, test_get_total_volume_four) {
    orderbook::queues::ring_buffer* ring_buffer = new orderbook::queues::ring_buffer{4};
    std::int64_t total_volume = ring_buffer->get_total_volume();
    EXPECT_EQ(total_volume, 0);
};

TEST(ring_buffer_test, test_reduce_tail) {
    orderbook::queues::ring_buffer* ring_buffer = new orderbook::queues::ring_buffer{4};
    ring_buffer->enqueue(2, 1, 99, 1, 88);
    ring_buffer->enqueue(2, 1, 2, 1, 88);
    ring_buffer->reduce_size_of_tail(2);
    orderbook::order* order = ring_buffer->dequeue();
    EXPECT_EQ(order->get_size(), 97);
};