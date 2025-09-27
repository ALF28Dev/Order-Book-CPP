
namespace orderbook {
    class tick_level
    {
        public:
            std::int64_t value;
            std::int64_t height;
            orderbook::tick_level* left;
            orderbook::tick_level* right;

            tick_level(std::int64_t value) : value(value), left(nullptr), right(nullptr), height(1) {}

            ~tick_level()
            {
                std::cout << "deleting price level.\n";
            }
    };
}
