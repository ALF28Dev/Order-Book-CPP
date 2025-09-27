
namespace orderbook::bitmaps {
    class mempool_bitmap
    {
        std::int64_t static constexpr BITMAP_SIZE = 10;

        private:
            std::int64_t bm[BITMAP_SIZE];

        public:
            mempool_bitmap()
            {
                for (int i = 0; i < BITMAP_SIZE; i++)
                {
                    bm[i] = ~0ULL;
                }
            }

            size_t aquire()
            {
                for( std::int64_t w = 0; w < BITMAP_SIZE; w++)
                {
                    std::int64_t x = bm[w];
                    if (x != 0)
                    {
                        std::int64_t b = __builtin_ctzll(x); 
                        bm[w] = x & (x - 1);
                        size_t complete_index = (w << 6) + b;
                        return complete_index;
                    };
                }
                return -1;
            }

            void release(std::int64_t index)
            {
                size_t w = index >> 6;
                size_t b = index & 63;
                bm[w] |= (1ULL << b);
            }
    };
}
