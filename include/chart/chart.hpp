#include <vector>
#include <cstdio>
#include <unordered_map>
#include <iostream>
using namespace std;

class Chart {
private:
    std::vector<int> cumulativeBidVolume;
    std::vector<int> cumulativeAskVolume;
    std::vector<int> bidPriceLevels;
    std::vector<int> askPriceLevels;
    std::vector<int> allPriceLevels;

    int calculateChartUpperLimit() {
        int maxCumulativeBidVolume = this->cumulativeBidVolume.empty() ? 0 : 
                        *std::max_element(this->cumulativeBidVolume.begin(), this->cumulativeBidVolume.end());
        int maxCumulativeAskVolume = this->cumulativeAskVolume.empty() ? 0 : 
                        *std::max_element(this->cumulativeAskVolume.begin(), this->cumulativeAskVolume.end());
        return std::max(maxCumulativeBidVolume, maxCumulativeAskVolume) * 1.1;
    }

public:
    Chart(std::vector<int> cumulativeBidVolume, std::vector<int> cumulativeAskVolume, std::vector<int> bidPriceLevels, std::vector<int> askPriceLevels) : cumulativeBidVolume(cumulativeBidVolume), cumulativeAskVolume(cumulativeAskVolume) {
        for (int priceLevel : bidPriceLevels) {
            this->allPriceLevels.push_back(priceLevel);
        }
        this->allPriceLevels.push_back(-1);
        for (int priceLevel : askPriceLevels) {
            this->allPriceLevels.push_back(priceLevel);
        }
        std::unordered_map<int, int> priceLevelMap;
        int i = 0;
        for (int priceLevel : allPriceLevels) {
            priceLevelMap[priceLevel] = ++i;
        }
        for (int priceLevel : bidPriceLevels) {
            this->bidPriceLevels.push_back(priceLevelMap[priceLevel]);
        }
        for (int priceLevel : askPriceLevels) {
            this->askPriceLevels.push_back(priceLevelMap[priceLevel]);
        }
    }

    void plot() {
        FILE* plotPipe = popen("gnuplot -persistent", "w");
        fprintf(plotPipe, "set terminal qt size 800,600\n");
        fprintf(plotPipe, "set title 'Order Book Depth'\n");
        fprintf(plotPipe, "set xlabel 'Price Levels (Bids & Asks)'\n");
        fprintf(plotPipe, "set ylabel 'Cumulative Volume'\n");
        fprintf(plotPipe, "set boxwidth 0.8 relative\n");
        fprintf(plotPipe, "set xrange [0.5:%zu.5]\n", size(this->allPriceLevels));
        fprintf(plotPipe, "set yrange [0:%d]\n", calculateChartUpperLimit());
        fprintf(plotPipe, "set xtics (");
        for (int i = 0; i < this->allPriceLevels.size(); ++i) {
            fprintf(plotPipe, "\"%d\" %d", this->allPriceLevels[i], i + 1);
            if (i != size(this->allPriceLevels) - 1) {
                fprintf(plotPipe, ", ");
            }
        }
        fprintf(plotPipe, ")\n");
        fprintf(plotPipe, "set style fill solid 0.5 border -1\n");
        fprintf(plotPipe, "plot '-' using 1:2 with boxes lc rgb 'green' title 'Bids', '-' using 1:2 with boxes lc rgb 'red' title 'Asks'\n");
        for (int i = 0; i < size(cumulativeBidVolume); ++i) {
            fprintf(plotPipe, "%d %d\n", bidPriceLevels[i], cumulativeBidVolume[i]);
        }
        fprintf(plotPipe, "e\n");
        for (int i = 0; i < size(cumulativeAskVolume); ++i) {
            fprintf(plotPipe, "%d %d\n", askPriceLevels[i], cumulativeAskVolume[i]);
        }
        fprintf(plotPipe, "e\n");
        fflush(plotPipe);
        pclose(plotPipe);
    }
};