// Quick Snippet: Statistics & Analysis
// Compile: g++ -O3 -march=native -std=c++23 -I../include -fopenmp stats.cpp -o stats && ./stats

#include <iostream>
#include <iomanip>
#include <cmath>
#include "csb/tensor.hpp"

int main() {
    csb::Tensor<float> data({10000});
    
    // Fill with some pseudo-random-like values
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = (float)((i * 17 + 42) % 1000) / 10.0f;
    }

    float min_val = data.min();
    float max_val = data.max();
    float mean_val = data.mean();
    float sum_val = data.sum();

    // Compute standard deviation
    float variance = 0.0f;
    for (size_t i = 0; i < data.size(); ++i) {
        float diff = data[i] - mean_val;
        variance += diff * diff;
    }
    variance /= data.size();
    float stdev = std::sqrt(variance);

    std::cout << "Statistical Analysis (10K elements)\n";
    std::cout << "====================================\n\n";
    std::cout << "Min:     " << std::fixed << std::setprecision(4) << min_val << "\n";
    std::cout << "Max:     " << max_val << "\n";
    std::cout << "Mean:    " << mean_val << "\n";
    std::cout << "Std Dev: " << stdev << "\n";
    std::cout << "Sum:     " << sum_val << "\n";

    return 0;
}
