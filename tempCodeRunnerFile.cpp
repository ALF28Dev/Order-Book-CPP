#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>

int main() {

    std::vector<int> bids = {25, 20, 10, 5};
    std::vector<int> asks = {0, 3, 7, 15, 21};
    std::vector<int> bid_categories = {203, 206, 207, 210};
    std::vector<int> ask_categories = {215, 216, 217, 218, 220};

    // Find the maximum value for the y-axis range
    int max_value = std::max(*std::max_element(bids.begin(), bids.end()), *std::max_element(asks.begin(), asks.end()));
    int upper_limit = max_value + max_value * 0.1; // Add a 10% buffer

    // Open a pipe to Gnuplot
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

    if (gnuplotPipe) {
        // Set Gnuplot output and style settings
        fprintf(gnuplotPipe, "set terminal qt size 800,600\n");
        fprintf(gnuplotPipe, "set title 'Order Book Depth'\n");
        fprintf(gnuplotPipe, "set xlabel 'Price Levels (Bids & Asks)'\n");
        fprintf(gnuplotPipe, "set ylabel 'Cumulative Volume'\n");
        fprintf(gnuplotPipe, "set xrange [%d:%d]\n", bid_categories.front() - 5, ask_categories.back() + 5);
        fprintf(gnuplotPipe, "set yrange [0:%d]\n", upper_limit);  // Set y-axis with buffer

        // Set style for boxes
        fprintf(gnuplotPipe, "set style fill solid 0.5 border -1\n");
        fprintf(gnuplotPipe, "set boxwidth 1 absolute\n");

        // Plot command for bids and asks using boxes
        fprintf(gnuplotPipe, "plot '-' using 1:2 with boxes lc rgb 'blue' title 'Bids', '-' using 1:2 with boxes lc rgb 'red' title 'Asks'\n");

        // Send bids data to Gnuplot: x = bid_categories, y = bids
        for (size_t i = 0; i < bids.size(); ++i) {
            fprintf(gnuplotPipe, "%d %d\n", bid_categories[i], bids[i]);
        }
        fprintf(gnuplotPipe, "e\n");

        // Send asks data to Gnuplot: x = ask_categories, y = asks
        for (size_t i = 0; i < asks.size(); ++i) {
            fprintf(gnuplotPipe, "%d %d\n", ask_categories[i], asks[i]);
        }
        fprintf(gnuplotPipe, "e\n");

        fflush(gnuplotPipe); // Ensure all data is sent to Gnuplot
        pclose(gnuplotPipe); // Close the pipe
    } else {
        std::cerr << "Gnuplot not found!" << std::endl;
    }

    return 0;
}
