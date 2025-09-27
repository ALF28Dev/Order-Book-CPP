#include <gtest/gtest.h>
#include <orderbook/bitmaps/mempool_bitmap.h>

TEST(mempool_bitmap_test, test_aquire_release_one) {
    orderbook::bitmaps::mempool_bitmap* bitmap = new orderbook::bitmaps::mempool_bitmap{};
    EXPECT_EQ(bitmap->aquire(),0);
    EXPECT_EQ(bitmap->aquire(),1);
    EXPECT_EQ(bitmap->aquire(),2);
};

TEST(mempool_bitmap_test, test_aquire_release_two) {
    orderbook::bitmaps::mempool_bitmap* bitmap = new orderbook::bitmaps::mempool_bitmap{};
    bitmap->aquire();
    bitmap->aquire();
    bitmap->aquire();
    bitmap->release(1);
    EXPECT_EQ(bitmap->aquire(),1);
};

TEST(mempool_bitmap_test, test_aquire_release_three) {
    orderbook::bitmaps::mempool_bitmap* bitmap = new orderbook::bitmaps::mempool_bitmap{};
    bitmap->aquire();
    bitmap->aquire();
    bitmap->aquire();
    bitmap->release(2);
    bitmap->release(0);
    EXPECT_EQ(bitmap->aquire(),0);
};

TEST(mempool_bitmap_test, test_aquire_release_four) {
    orderbook::bitmaps::mempool_bitmap* bitmap = new orderbook::bitmaps::mempool_bitmap{};
    bitmap->aquire();
    bitmap->aquire();
    bitmap->aquire();
    bitmap->release(2);
    bitmap->release(0);
    bitmap->aquire();
    EXPECT_EQ(bitmap->aquire(),2);
};
