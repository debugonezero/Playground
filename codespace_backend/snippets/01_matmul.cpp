// Quick Snippet: Matrix Multiplication Benchmark
// Compile: g++ -O3 -march=native -std=c++23 -I../include -fopenmp matmul.cpp -o matmul && ./matmul

#include <iostream>
#include <chrono>
#include <iomanip>
#include "csb/tensor.hpp"

int main() {
    std::cout << "Matrix Multiplication Benchmark\n";
    std::cout << "===============================\n\n";

    for (int dim = 128; dim <= 1024; dim *= 2) {
        csb::Tensor<float> A({dim, dim});
        csb::Tensor<float> B({dim, dim});
        A.fill(1.0f);
        B.fill(2.0f);

        auto start = std::chrono::high_resolution_clock::now();
        auto C = A.matmul(B);
        auto end = std::chrono::high_resolution_clock::now();

        double time_ms = (end - start).count() * 1000.0;
        double ops = 2.0 * dim * dim * dim;
        double gflops = ops / ((end - start).count() * 1e9);

        std::cout << "Matrix " << std::setw(4) << dim << "x" << std::setw(4) << dim
                  << "  Time: " << std::setw(8) << std::fixed << std::setprecision(3) << time_ms
                  << " ms   GFLOPS: " << gflops << "\n";
    }

    return 0;
}
