

class PriceLevel {
public:
    int price_level;
    PriceLevel* left;
    PriceLevel* right;
    int height = 1;
    PriceLevel(int price_level) : price_level(price_level) {}
};
