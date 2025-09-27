#include <gtest/gtest.h>
#include <orderbook/bitmaps/tick_level_bitmap.h>

TEST(tick_level_bitmap_test, test_set_is_set_one) {
    orderbook::bitmaps::tick_level_bitmap* bitmap = new orderbook::bitmaps::tick_level_bitmap{};
    bitmap->set(1);
    EXPECT_EQ(bitmap->is_set(1),true);
};

TEST(tick_level_bitmap_test, test_set_is_set_two) {
    orderbook::bitmaps::tick_level_bitmap* bitmap = new orderbook::bitmaps::tick_level_bitmap{};
    EXPECT_EQ(bitmap->is_set(1),false);
    EXPECT_EQ(bitmap->is_set(2),false);
    EXPECT_EQ(bitmap->is_set(3),false);
};

TEST(tick_level_bitmap_test, test_set_is_set_three) {
    orderbook::bitmaps::tick_level_bitmap* bitmap = new orderbook::bitmaps::tick_level_bitmap{};
    bitmap->set(1);
    bitmap->unset(1);
    EXPECT_EQ(bitmap->is_set(1),false);
};

TEST(tick_level_bitmap_test, test_set_is_set_four) {
    orderbook::bitmaps::tick_level_bitmap* bitmap = new orderbook::bitmaps::tick_level_bitmap{};
    bitmap->unset(1);
    EXPECT_EQ(bitmap->is_set(1),false);
};

TEST(tick_level_bitmap_test, test_set_is_set_five) {
    orderbook::bitmaps::tick_level_bitmap* bitmap = new orderbook::bitmaps::tick_level_bitmap{};
    bitmap->set(1);
    bitmap->set(1);
    EXPECT_EQ(bitmap->is_set(1),true);
};