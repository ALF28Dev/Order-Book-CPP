#include <iostream>
#include "orderbook/tick_level/tick_level.h"
#include "orderbook/bitmaps/mempool_bitmap.h"
#include "orderbook/bitmaps/tick_level_bitmap.h"
#include "orderbook/maps/order_map.h"

namespace orderbook::trees {

    class avl_tree
    {
        private:

            orderbook::tick_level* memory_pool;
            orderbook::bitmaps::mempool_bitmap* mp_bm;
            orderbook::bitmaps::tick_level_bitmap* tl_bm;
            orderbook::tick_level* root;
            std::int64_t n_tick_levels;
            
            std::int64_t get_height(orderbook::tick_level* tick_level)
            {
                return tick_level ? tick_level->height : 0;
            }

            std::int64_t get_max_child_height(orderbook::tick_level* tick_level)
            {
                return std::max(get_height(tick_level->left), get_height(tick_level->right));
            }

            std::int64_t get_balance_factor(orderbook::tick_level* tick_level)
            {
                return tick_level ? get_height(tick_level->left) - get_height(tick_level->right) : 0;
            }

            orderbook::tick_level* rotate_left(orderbook::tick_level* tick_level)
            {
                orderbook::tick_level* right = tick_level->right;
                orderbook::tick_level* right_left_child = right->left;
                right->left = tick_level;
                tick_level->right = right_left_child;
                tick_level->height = 1 + get_max_child_height(tick_level);
                right->height = 1 + get_max_child_height(right);
                return right;
            }

            orderbook::tick_level* rotate_right(orderbook::tick_level* tick_level)
            {
                orderbook::tick_level* left = tick_level->left;
                orderbook::tick_level* left_right_child = left->right;
                left->right = tick_level;
                tick_level->left = left_right_child;
                tick_level->height = 1 + get_max_child_height(tick_level);
                left->height = 1 + get_max_child_height(left);
                return left;
            }

            orderbook::tick_level* insert(orderbook::tick_level* tl, int tick_level)
            {
                if (tl == nullptr)
                {
                    size_t free_index = mp_bm->aquire();
                    if (free_index < 0 || free_index >= n_tick_levels)
                    {
                        std::cout << "NO FREE PRICE LEVELS MUST ALLOCATE ADDITIONAL SPACE.\n";
                        return nullptr;
                    };
                    orderbook::tick_level* free_level = (memory_pool+free_index);
                    free_level->value = tick_level; // set tick_level value of reused node;
                    free_level->left = free_level->right = nullptr; // ensure left and right of reused node are nullptr;
                    free_level->height = 1;
                    tl_bm->set(tick_level);
                    return free_level;
                } else if (tick_level < tl->value){
                    tl->left = insert(tl->left, tick_level);
                } else {
                    tl->right = insert(tl->right, tick_level);
                }
                tl->height = 1 + get_max_child_height(tl);
                int balance_factor = get_balance_factor(tl);
                if (balance_factor > 1 && tick_level < tl->left->value)
                {
                    return rotate_right(tl);
                }
                if (balance_factor < -1 && tick_level > tl->right->value)
                {
                    return rotate_left(tl);
                }
                if (balance_factor > 1 && tick_level > tl->left->value)
                {
                    tl->left = rotate_left(tl->left);
                    return rotate_right(tl);
                }
                if (balance_factor < -1 && tick_level < tl->right->value)
                {
                    tl->right = rotate_right(tl->right);
                    return rotate_left(tl);
                }
                return tl;
            }

            orderbook::tick_level* remove(orderbook::tick_level* tl, int tick_level)
            {
                if (tl == nullptr) {
                    return tl;
                }
                if (tick_level < tl->value) {
                    tl->left = remove(tl->left, tick_level);
                } else if (tick_level > tl->value) {
                    tl->right = remove(tl->right, tick_level);
                } else {
                    if (!tl->left || !tl->right)
                    {
                        orderbook::tick_level* temp = tl->left ? tl->left : tl->right;
                        std::size_t index = tl - memory_pool;
                        tl->value = -1;
                        mp_bm->release(index);
                        tl_bm->unset(tick_level);
                        std::cout << "RELEASING NODE\n";
                        return temp;
                    }
                    orderbook::tick_level* tmp = get_min(tl->right);
                    tl->value = tmp->value;
                    tl->right = remove(tl->right, tmp->value);
                }
                tl->height = 1 + this->get_max_child_height(tl);
                int balance_factor = get_balance_factor(tl);
                if (balance_factor > 1 && get_balance_factor(tl->left) >= 0)
                {
                    return rotate_right(tl);
                }
                if (balance_factor < -1 && get_balance_factor(tl->right) <= 0)
                {
                    return rotate_left(tl);
                }
                if (balance_factor > 1 && get_balance_factor(tl->left) < 0)
                {
                    tl->left = rotate_left(tl->left);
                    return rotate_right(tl);
                }
                if (balance_factor < -1 && get_balance_factor(tl->right) > 0)
                {
                    tl->right = rotate_right(tl->right);
                    return rotate_left(tl);
                }
                return tl;
            }

            orderbook::tick_level* get_min(orderbook::tick_level* root)
            {
                orderbook::tick_level* current = root;
                while (current && current->left)
                    current = current->left;
                return current;
            }

            orderbook::tick_level* get_max(orderbook::tick_level* root)
            {
                orderbook::tick_level* current = root;
                while (current && current->right)
                    current = current->right;
                return current;
            }

            orderbook::tick_level* get_min_tick_level()
            {
                return get_min(root);
            }

            orderbook::tick_level* get_max_tick_level()
            {
                return get_max(root);
            }

            std::int64_t eager_allocation()
            {
                for(std::int64_t i = 0; i < n_tick_levels; i++)
                {
                    new (memory_pool+i) orderbook::tick_level{-1};
                    std::cout << "allocating tick level.\n";
                };
                return 0;
            }

            void traverse(orderbook::tick_level* curr)
            {
                if(curr->left != nullptr)
                {
                    traverse(curr->left);
                }
                std::cout << "tree value: " << curr->value << ".\n";
                if(curr->right != nullptr)
                {
                    traverse(curr->right);
                }
            }

        public:
            avl_tree(std::int64_t n)
            {
                root = nullptr;
                n_tick_levels = n;
                memory_pool = static_cast<orderbook::tick_level*>(std::malloc(n * sizeof(orderbook::tick_level)));
                mp_bm = new orderbook::bitmaps::mempool_bitmap{};
                tl_bm = new orderbook::bitmaps::tick_level_bitmap{};
                eager_allocation();
            }

            std::int64_t get_min_value()
            {
                return get_min(root)->value;
            }

            std::int64_t get_max_value()
            {
                return get_max(root)->value;
            }


            bool contains(std::int64_t tick_level)
            {
                return tl_bm->is_set(tick_level);
            }

            void insert(std::int64_t tick_level)
            {
                if(tl_bm->is_set(tick_level))
                {
                    std::cout << "VALUE ALREADY IN TREE!\n";
                    return;
                }
                root = insert(root, tick_level);
            }

            void remove(std::int64_t tick_level)
            {
                root = remove(root, tick_level);
            }

            void remove_min()
            {
                root = remove(root, get_min_value());
            }

            void remove_max()
            {
                root = remove(root, get_max_value());
            }

            void print()
            {
                traverse(root);
            }

            bool is_empty() {
                return root == nullptr;
            }

            ~avl_tree()
            {
                for(std::int64_t i = 0; i < n_tick_levels; i++)
                {
                    (memory_pool+i)->~tick_level();
                };
                std::cout << "destroying tree, freeing memory.\n";
                std::free(memory_pool);
                delete mp_bm;
                delete tl_bm;
            }
    };
}
