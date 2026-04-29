#include <chrono>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "csb/tensor.hpp"

template<typename T>
void benchmark_operation(const std::string& op_name, const csb::Tensor<T>& a, const csb::Tensor<T>& b,
                         std::function<csb::Tensor<T>(const csb::Tensor<T>&, const csb::Tensor<T>&)> op) {
    auto start = std::chrono::high_resolution_clock::now();
    auto result = op(a, b);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    double ops = static_cast<double>(a.size());
    double gflops = ops / (elapsed.count() * 1e9);
    
    std::cout << "  " << std::left << std::setw(15) << op_name 
              << "  " << std::scientific << std::setprecision(3) << elapsed.count() 
              << " s    " << std::fixed << std::setprecision(2) << gflops << " GFLOPS\n";
}

int main() {
    std::cout << "=== Codespace Backend Benchmarks ===\n\n";

    // Benchmark float add on large arrays
    std::cout << "Float operations (size = 2^24 = 16M elements):\n";
    const std::size_t size_large = 1 << 24;
    csb::Tensor<float> f_a({size_large});
    csb::Tensor<float> f_b({size_large});
    f_a.fill(1.0f);
    f_b.fill(2.0f);

    benchmark_operation("ADD", f_a, f_b, [](const auto& a, const auto& b) { return a.add(b); });
    benchmark_operation("SUB", f_a, f_b, [](const auto& a, const auto& b) { return a.sub(b); });
    benchmark_operation("MUL", f_a, f_b, [](const auto& a, const auto& b) { return a.mul(b); });
    benchmark_operation("DIV", f_a, f_b, [](const auto& a, const auto& b) { return a.div(b); });

    std::cout << "\nDouble operations (size = 2^24):\n";
    csb::Tensor<double> d_a({size_large});
    csb::Tensor<double> d_b({size_large});
    d_a.fill(1.5);
    d_b.fill(2.5);

    benchmark_operation("ADD", d_a, d_b, [](const auto& a, const auto& b) { return a.add(b); });
    benchmark_operation("SUB", d_a, d_b, [](const auto& a, const auto& b) { return a.sub(b); });
    benchmark_operation("MUL", d_a, d_b, [](const auto& a, const auto& b) { return a.mul(b); });
    benchmark_operation("DIV", d_a, d_b, [](const auto& a, const auto& b) { return a.div(b); });

    // Benchmark matrix operations
    std::cout << "\nMatrix operations (square matrices 512x512):\n";
    csb::Tensor<float> mat_a({512, 512});
    csb::Tensor<float> mat_b({512, 512});
    mat_a.fill(1.0f);
    mat_b.fill(2.0f);

    auto start = std::chrono::high_resolution_clock::now();
    auto mat_result = mat_a.matmul(mat_b);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double ops = 2.0 * 512.0 * 512.0 * 512.0;
    double gflops = ops / (elapsed.count() * 1e9);
    std::cout << "  " << std::left << std::setw(15) << "MATMUL (512x512)" 
              << "  " << std::scientific << std::setprecision(3) << elapsed.count() 
              << " s    " << std::fixed << std::setprecision(2) << gflops << " GFLOPS\n";

    // Benchmark transpose
    std::cout << "\nTranspose operations (512x512):\n";
    start = std::chrono::high_resolution_clock::now();
    auto mat_t = mat_a.transpose();
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "  " << std::left << std::setw(15) << "TRANSPOSE (512x512)" 
              << "  " << std::scientific << std::setprecision(3) << elapsed.count() << " s\n";

    // Benchmark reductions
    std::cout << "\nReduction operations (2^24 elements):\n";
    start = std::chrono::high_resolution_clock::now();
    auto sum_result = f_a.sum();
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "  " << std::left << std::setw(15) << "SUM" 
              << "  " << std::scientific << std::setprecision(3) << elapsed.count() 
              << " s    Result: " << sum_result << "\n";

    start = std::chrono::high_resolution_clock::now();
    auto min_result = f_a.min();
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "  " << std::left << std::setw(15) << "MIN" 
              << "  " << std::scientific << std::setprecision(3) << elapsed.count() << " s\n";

    start = std::chrono::high_resolution_clock::now();
    auto max_result = f_a.max();
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "  " << std::left << std::setw(15) << "MAX" 
              << "  " << std::scientific << std::setprecision(3) << elapsed.count() << " s\n";

    std::cout << "\nBenchmark complete.\n";
    return 0;
}
