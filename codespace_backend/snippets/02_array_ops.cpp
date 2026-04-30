// Quick Snippet: Simple Array Operations
// Compile: g++ -O3 -march=native -std=c++23 -I../include -fopenmp array_ops.cpp -o array_ops && ./array_ops

#include <iostream>
#include "csb/tensor.hpp"

int main() {
    csb::Tensor<float> a({1000000});
    csb::Tensor<float> b({1000000});

    a.fill(2.5f);
    b.fill(3.5f);

    std::cout << "Array Operations on 1M elements\n";
    std::cout << "================================\n\n";

    auto sum = a.add(b);
    std::cout << "ADD: " << sum[0] << " (expected 6.0)\n";

    auto prod = a.mul(b);
    std::cout << "MUL: " << prod[0] << " (expected 8.75)\n";

    auto ratio = a.div(b);
    std::cout << "DIV: " << ratio[0] << " (expected 0.714)\n";

    float total = a.sum();
    std::cout << "\nSUM: " << total << "\n";
    std::cout << "MIN: " << a.min() << "\n";
    std::cout << "MAX: " << a.max() << "\n";
    std::cout << "MEAN: " << a.mean() << "\n";

    return 0;
}
