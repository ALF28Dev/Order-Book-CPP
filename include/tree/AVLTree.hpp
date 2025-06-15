#include <tree/priceLevel.hpp>
#include <set>
using namespace std;

/**
 * @class AVLTree
 * @brief Maintains a self-balancing binary search tree of PriceLevel nodes for efficient price-level management.
 *
 * The `AVLTree` class organizes price levels in sorted order and keeps the tree balanced
 * using AVL rotations. It allows efficient insertion, deletion, and retrieval of price levels,
 * ensuring logarithmic time complexity for each operation.
 *
 * The tree stores pointers to `PriceLevel` nodes and updates node height information after
 * modifications to preserve balance. When imbalance is detected, the appropriate single or
 * double rotation (left or right) is performed to restore the AVL property.
 *
 * ### Fields:
 * - **root** (`PriceLevel*`): Pointer to the root node of the AVL tree.
 *
 * ### Private Methods:
 * - **get_height** (`int get_height(PriceLevel* node)`): Returns the height of the given node, treating null as 0.
 * - **get_max_child_height** (`int get_max_child_height(PriceLevel* node)`): Computes the max height between a node's children.
 * - **get_balance_factor** (`int get_balance_factor(PriceLevel* node)`): Calculates the balance factor of a node.
 * - **rotate_left** (`PriceLevel* rotate_left(PriceLevel* node)`): Performs a left rotation around the specified node.
 * - **rotate_right** (`PriceLevel* rotate_right(PriceLevel* node)`): Performs a right rotation around the specified node.
 *
 * ### Public Methods:
 * - **AVLTree()**: Constructs an empty AVL tree.
 * - **get_min_price_level** (`PriceLevel* get_min_price_level(PriceLevel* node)`): Returns the node with the smallest value in a subtree.
 * - **get_max_price_level** (`PriceLevel* get_max_price_level(PriceLevel* node)`): Returns the node with the largest value in a subtree.
 * - **insert** (`PriceLevel* insert(PriceLevel* root, int price_level)`): Inserts a new price level into the tree, rebalancing as needed.
 * - **remove** (`PriceLevel* remove(PriceLevel* root, int price_level)`): Removes the given price level, rebalances the tree.
 */
class AVLTree {
private:
    PriceLevel* root;
    int size = 0;
    std::set<int> price_levels;

    /**
     * @brief Returns the height of a specified node.
     * 
     * @param price_level The price_level of the subtree for which we want the height.
     * @return Integer value of the height of the subtree from the price_level.
     */
    int get_height(PriceLevel* price_level) {
        return price_level ? price_level->height : 0;
    }

    /**
     * @brief Returns the max height of the left or right subtrees.
     * 
     * @param price_level The root of the specified subtree.
     * @return Integer value of the max height of the left or right subtree.
     */
    int get_max_child_height(PriceLevel* price_level) {
        return std::max(get_height(price_level->left), get_height(price_level->right));
    }

    /**
     * @brief Returns the balance factor of the specified root node.
     * 
     * @param price_level The root of the specified subtree which we want to source the balance factor.
     * @return Integer value of the height of the left subtree minus the height of the right subtree.
     */
    int get_balance_factor(PriceLevel* price_level) {
        return price_level ? get_height(price_level->left) - get_height(price_level->right) : 0;
    }

    /**
     * @brief Rotates left around the root of the provided subtree.
     * 
     * @param price_level The root of the specified subtree which we want to rotate around to th left.
     * @return The new root of the rotated subtree.
     */
    PriceLevel* rotate_left(PriceLevel* price_level) {
        PriceLevel* right = price_level->right;
        PriceLevel* right_left_child = right->left;

        right->left = price_level;
        price_level->right = right_left_child;

        price_level->height = 1 + this->get_max_child_height(price_level);
        right->height = 1 + this->get_max_child_height(right);
        return right;
    }

    /**
     * @brief Rotates right around the root of the provided subtree.
     * 
     * @param price_level The root of the specified subtree which we want to rotate around to th right.
     * @return The new root of the rotated subtree.
     */
    PriceLevel* rotate_right(PriceLevel* price_level) {
        PriceLevel* left = price_level->left;
        PriceLevel* left_right_child = left->right;

        left->right = price_level;
        price_level->left = left_right_child;

        price_level->height = 1 + this->get_max_child_height(price_level);
        left->height = 1 + this->get_max_child_height(left);
        return left;
    }

    /**
     * @brief Insert a node with a specified price level into the AVL tree.
     * 
     * @param root The root of the AVL tree.
     * @param price_level The int price level to be inserted into the tree.
     * @return The root node.
     */
    PriceLevel* insert(PriceLevel* new_price_level, int price_level) {
        if (new_price_level == nullptr)
        {
            this->size++;
            this->price_levels.insert(price_level);
            return new PriceLevel(price_level);
        } else if (price_level < new_price_level->price_level)
        {
            new_price_level->left = this->insert(new_price_level->left, price_level);
        } else
        {
            new_price_level->right = this->insert(new_price_level->right, price_level);
        }
        new_price_level->height = 1 + this->get_max_child_height(new_price_level);
        int balance_factor = this->get_balance_factor(new_price_level);
        if (balance_factor > 1 && price_level < new_price_level->left->price_level)
        {
            return this->rotate_right(new_price_level);
        }
        if (balance_factor < -1 && price_level > new_price_level->right->price_level)
        {
            return this->rotate_left(new_price_level);
        }
        if (balance_factor > 1 && price_level > new_price_level->left->price_level)
        {
            new_price_level->left = this->rotate_left(new_price_level->left);
            return this->rotate_right(new_price_level);
        }
        if (balance_factor < -1 && price_level < new_price_level->right->price_level)
        {
            new_price_level->right = this->rotate_right(new_price_level->right);
            return this->rotate_left(new_price_level);
        }
        return new_price_level;
    }

    /**
     * @brief Remove a price level from the AVL tree with the specified int price level.
     * 
     * @param new_price_level The root of the AVL tree.
     * @param price_level The int price level to be removed from the tree.
     * @return The root node.
     */
    PriceLevel* remove(PriceLevel* new_price_level, int price_level) {
        if (new_price_level == nullptr) {
            return new_price_level;
        }
        if (price_level < new_price_level->price_level) {
            new_price_level->left = remove(new_price_level->left, price_level);
        } else if (price_level > new_price_level->price_level) {
            new_price_level->right = remove(new_price_level->right, price_level);
        } else {
            if (!new_price_level->left || !new_price_level->right) {
                PriceLevel* temp = new_price_level->left ? new_price_level->left : new_price_level->right;
                delete new_price_level;
                this->size--;
                this->price_levels.erase(price_level);
                return temp;
            }
            PriceLevel* temp = get_min_price_level(new_price_level->right);
            new_price_level->price_level = temp->price_level;
            new_price_level->right = remove(new_price_level->right, temp->price_level);
        }
        new_price_level->height = 1 + this->get_max_child_height(new_price_level);
        int balance_factor = get_balance_factor(new_price_level);
        if (balance_factor > 1 && get_balance_factor(new_price_level->left) >= 0)
        {
            return rotate_right(new_price_level);
        }
        if (balance_factor < -1 && get_balance_factor(new_price_level->right) <= 0)
        {
            return rotate_left(new_price_level);
        }
        if (balance_factor > 1 && get_balance_factor(new_price_level->left) < 0)
        {
            new_price_level->left = rotate_left(new_price_level->left);
            return rotate_right(new_price_level);
        }
        if (balance_factor < -1 && get_balance_factor(new_price_level->right) > 0)
        {
            new_price_level->right = rotate_right(new_price_level->right);
            return rotate_left(new_price_level);
        }
        return new_price_level;
    }

public:
    AVLTree() : root(nullptr) {}

    /**
     * @brief Source the node with the minimum price.
     * 
     * @param root The root of the AVL tree.
     * @return The node with the minimum price.
     */
    PriceLevel* get_min_price_level(PriceLevel* root) {
        PriceLevel* current = root;
        while (current && current->left)
            current = current->left;
        return current;
    }

    /**
     * @brief Source the node with the maximum price.
     * 
     * @param root The root of the AVL tree.
     * @return The node with the maximum price.
     */
    PriceLevel* get_max_price_level(PriceLevel* root) {
        PriceLevel* current = root;
        while (current && current->right)
            current = current->right;
        return current;
    }

    /**
     * @brief Source the price of the minimum price level.
     * 
     * @return The int price.
     */
    int min() {
        PriceLevel* min_node = get_min_price_level(root);
        return min_node ? min_node->price_level : -1;
    }

    /**
     * @brief Source the price of the maximum price level.
     * 
     * @return The int price.
     */
    int max() {
        PriceLevel* max_node = get_max_price_level(root);
        return max_node ? max_node->price_level : -1;
    }
    
    /**
     * @brief Create a price level for a specified int price value.
     * 
     * @param price_level an int value representing a price level.
     */  
    void insert(int price_level) {
        if (this->price_levels.count(price_level)) return;
        this->root = this->insert(this->root, price_level);
    }

    /**
     * @brief Remove a specified price level from the tree.
     * 
     * @param price_level an int value representing a price level.
     */  
    void remove(int price_level) {
        if (!this->price_levels.count(price_level)) return;
        this->root = this->remove(this->root, price_level);
    }

    /**
     * @brief Check if the tree has no price levels.
     * 
     * @return A int of the number of price levels in the tree, e.g. the depth of the book.
     */
    int get_depth() {
        return this->size;
    }

    /**
     * @brief Check if the tree has no price levels.
     * 
     * @return A bool of true or false depending on if the tree is empty.
     */
    bool is_empty() {
       return root == nullptr;
    }

    /**
     * @brief Produce an inorder collection of the price levels in the tree.
     * 
     * @param PriceLevel current price_level being traversed.
     * @param set A set containing the values of the levels in the tree.
     */  
    void inorder_collect(PriceLevel* price_level, std::set<int>& out) {
        if (!price_level) return;
        inorder_collect(price_level->left, out);
        out.insert(price_level->price_level);
        inorder_collect(price_level->right, out);
    }

    /**
     * @brief Source an inorder collection of price levels and return a set containing the result.
     * 
     * @return A set containing an inorder collection of price levels.
     */   
    std::set<int>* getAllLevels() {
        auto* levels = new std::set<int>();
        inorder_collect(this->root, *levels);
        return levels;
    }
};
