// Quick Snippet: Data Processing Pipeline
// Compile: g++ -O3 -march=native -std=c++23 -I../include -fopenmp pipeline.cpp -o pipeline && ./pipeline

#include <iostream>
#include <iomanip>
#include "csb/tensor.hpp"

int main() {
    std::cout << "Data Processing Pipeline\n";
    std::cout << "========================\n\n";

    // Step 1: Load data (simulated)
    csb::Tensor<float> raw_data({1000, 100});
    raw_data.fill(5.0f);  // Simulate loading data

    std::cout << "Step 1: Loaded data [" << raw_data.shape()[0] 
              << ", " << raw_data.shape()[1] << "]\n";

    // Step 2: Normalize (simple linear normalization)
    float data_mean = raw_data.mean();
    float data_min = raw_data.min();
    float data_max = raw_data.max();
    
    std::cout << "Step 2: Statistics\n";
    std::cout << "  Mean: " << std::fixed << std::setprecision(3) << data_mean << "\n";
    std::cout << "  Min:  " << data_min << "\n";
    std::cout << "  Max:  " << data_max << "\n\n";

    // Step 3: Feature engineering (matrix multiply)
    csb::Tensor<float> feature_matrix({raw_data.shape()[1], 50});
    feature_matrix.fill(0.01f);  // Random-like weights

    auto features = raw_data.matmul(feature_matrix);
    std::cout << "Step 3: Feature extraction [" << features.shape()[0]
              << ", " << features.shape()[1] << "]\n";

    // Step 4: Output statistics
    std::cout << "Step 4: Output statistics\n";
    std::cout << "  Mean: " << features.mean() << "\n";
    std::cout << "  Min:  " << features.min() << "\n";
    std::cout << "  Max:  " << features.max() << "\n";

    std::cout << "\n✅ Pipeline complete\n";
    return 0;
}
