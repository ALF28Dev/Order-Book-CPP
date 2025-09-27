
namespace orderbook::bitmaps {
    class tick_level_bitmap
    {
        std::int64_t static constexpr BITMAP_SIZE = 15625;

        private:
            std::int64_t bm[BITMAP_SIZE];

        public:
            tick_level_bitmap()
            {
                for (int i = 0; i < BITMAP_SIZE; i++)
                {
                    bm[i] = 0;
                }
            }

            void unset(std::int64_t index)
            {
                size_t w = index >> 6; // word index = index / 64
                size_t b = index & 63; // bit index  = index % 64
                bm[w] &= ~(1ULL << b);
            }

            void set(std::int64_t index)
            {
                size_t w = index >> 6;
                size_t b = index & 63;
                bm[w] |= 1ULL << b;
            }

            bool is_set(std::int64_t index)
            {
                size_t w = index >> 6;
                size_t b = index & 63;
                return (bm[w] & (1ULL << b)) != 0;
            }
    };
}
