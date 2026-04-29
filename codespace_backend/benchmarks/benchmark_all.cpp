#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include "csb/tensor.hpp"

struct BenchResult {
    std::string name;
    double elapsed_sec;
    double gflops;
};

int main() {
    std::cout << "=== Comprehensive Codespace Backend Benchmark ===\n\n";

    std::vector<BenchResult> results;

    // Float elementwise operations
    std::cout << "Running float benchmarks...\n";
    for (int size_exp = 20; size_exp <= 24; size_exp += 2) {
        std::size_t size = 1 << size_exp;
        csb::Tensor<float> a({size});
        csb::Tensor<float> b({size});
        a.fill(1.0f);
        b.fill(2.0f);

        auto start = std::chrono::high_resolution_clock::now();
        auto result = a.add(b);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        double gflops = size / (elapsed.count() * 1e9);
        
        char buf[256];
        snprintf(buf, sizeof(buf), "float ADD (2^%d)", size_exp);
        results.push_back({buf, elapsed.count(), gflops});
    }

    // Double elementwise operations
    std::cout << "Running double benchmarks...\n";
    for (int size_exp = 20; size_exp <= 24; size_exp += 2) {
        std::size_t size = 1 << size_exp;
        csb::Tensor<double> a({size});
        csb::Tensor<double> b({size});
        a.fill(1.5);
        b.fill(2.5);

        auto start = std::chrono::high_resolution_clock::now();
        auto result = a.mul(b);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        double gflops = size / (elapsed.count() * 1e9);
        
        char buf[256];
        snprintf(buf, sizeof(buf), "double MUL (2^%d)", size_exp);
        results.push_back({buf, elapsed.count(), gflops});
    }

    // Integer operations
    std::cout << "Running int32 benchmarks...\n";
    std::size_t int_size = 1 << 22;
    csb::Tensor<int32_t> i32_a({int_size});
    csb::Tensor<int32_t> i32_b({int_size});
    i32_a.fill(100);
    i32_b.fill(50);

    auto start = std::chrono::high_resolution_clock::now();
    auto i32_add = i32_a.add(i32_b);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double gflops = int_size / (elapsed.count() * 1e9);
    results.push_back({"int32 ADD (2^22)", elapsed.count(), gflops});

    // Matrix multiplication benchmarks
    std::cout << "Running matmul benchmarks...\n";
    for (int dim = 128; dim <= 512; dim *= 2) {
        csb::Tensor<float> mat_a({dim, dim});
        csb::Tensor<float> mat_b({dim, dim});
        mat_a.fill(1.0f);
        mat_b.fill(2.0f);

        auto start_mm = std::chrono::high_resolution_clock::now();
        auto result = mat_a.matmul(mat_b);
        auto end_mm = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_mm = end_mm - start_mm;
        
        double ops = 2.0 * dim * dim * dim;
        double gflops_mm = ops / (elapsed_mm.count() * 1e9);
        
        char buf[256];
        snprintf(buf, sizeof(buf), "MATMUL (%dx%d)", dim, dim);
        results.push_back({buf, elapsed_mm.count(), gflops_mm});
    }

    // Print results table
    std::cout << "\n=== Results ===\n";
    std::cout << std::left << std::setw(30) << "Operation"
              << std::right << std::setw(15) << "Time (s)"
              << std::right << std::setw(15) << "Performance" << "\n";
    std::cout << std::string(60, '-') << "\n";

    for (const auto& r : results) {
        std::cout << std::left << std::setw(30) << r.name
                  << std::right << std::scientific << std::setprecision(3) << std::setw(15) << r.elapsed_sec
                  << std::right << std::fixed << std::setprecision(2) << std::setw(13) << r.gflops << " GFLOPS\n";
    }

    std::cout << "\nBenchmark complete.\n";
    return 0;
}
