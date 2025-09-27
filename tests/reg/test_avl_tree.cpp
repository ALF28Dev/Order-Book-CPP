#include <gtest/gtest.h>
#include <orderbook/trees/avl_tree.h>

TEST(avl_tree_test, test_is_emtpy) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    EXPECT_EQ(tree->is_empty(), true);
};

TEST(avl_tree_test, test_insert_one) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    tree->insert(1);
    EXPECT_EQ(tree->is_empty(), false);
};

TEST(avl_tree_test, test_insert_three) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    tree->insert(1);
    tree->remove(1);
    EXPECT_EQ(tree->is_empty(), true);
};

TEST(avl_tree_test, test_insert_four) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    tree->insert(1);
    tree->remove_min();
    EXPECT_EQ(tree->is_empty(), true);
};

TEST(avl_tree_test, test_insert_five) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    tree->insert(1);
    tree->remove_max();
    EXPECT_EQ(tree->is_empty(), true);
};

TEST(avl_tree_test, test_min_one) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    tree->insert(2);
    tree->insert(4);
    tree->insert(1);
    tree->insert(3);
    EXPECT_EQ(tree->get_min_value(), 1);
};

TEST(avl_tree_test, test_min_two) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    tree->insert(2);
    tree->insert(4);
    tree->insert(1);
    tree->insert(3);
    tree->remove(1);
    EXPECT_EQ(tree->get_min_value(), 2);
};

TEST(avl_tree_test, test_min_three) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    tree->insert(2);
    tree->insert(4);
    tree->insert(1);
    tree->insert(3);
    tree->remove_min();
    tree->remove_min();
    tree->remove_min();
    EXPECT_EQ(tree->get_min_value(), 4);
};

TEST(avl_tree_test, test_min_four) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    tree->insert(2);
    tree->insert(4);
    tree->insert(1);
    tree->insert(3);
    tree->remove_min();
    tree->remove_min();
    tree->remove_max();
    EXPECT_EQ(tree->get_min_value(), 3);
};

TEST(avl_tree_test, test_contains_one) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    tree->insert(2);
    tree->insert(4);
    tree->insert(1);
    tree->insert(3);
    tree->remove_min();
    tree->remove_min();
    tree->remove_max();
    EXPECT_EQ(tree->contains(3), true);
};

TEST(avl_tree_test, test_contains_two) {
    orderbook::trees::avl_tree* tree = new orderbook::trees::avl_tree{10};
    tree->insert(2);
    tree->insert(4);
    tree->insert(1);
    tree->insert(3);
    tree->remove_min();
    tree->remove_min();
    tree->remove_max();
    EXPECT_EQ(tree->contains(1), false);
};