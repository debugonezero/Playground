#include <chrono>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include "csb/tensor.hpp"

// Simple neural network forward pass demo
class SimpleNN {
public:
    csb::Tensor<float> weights1;
    csb::Tensor<float> bias1;
    csb::Tensor<float> weights2;
    csb::Tensor<float> bias2;

    SimpleNN() : weights1({784, 128}), bias1({128}), 
                 weights2({128, 10}), bias2({10}) {
        weights1.fill(0.01f);
        bias1.fill(0.0f);
        weights2.fill(0.01f);
        bias2.fill(0.0f);
    }

    csb::Tensor<float> forward(const csb::Tensor<float>& input) {
        // Layer 1: matmul
        auto hidden = input.matmul(weights1);  // [1, 128]
        
        // Add bias (expand bias to match hidden shape for manual broadcasting)
        for (std::size_t i = 0; i < hidden.size(); ++i) {
            hidden[i] = hidden[i] + bias1[i];
        }
        
        // Simple ReLU: clamp negative values
        for (std::size_t i = 0; i < hidden.size(); ++i) {
            if (hidden[i] < 0.0f) {
                hidden[i] = 0.0f;
            }
        }

        // Layer 2: matmul
        auto output = hidden.matmul(weights2);  // [1, 10]
        
        // Add bias manually
        for (std::size_t i = 0; i < output.size(); ++i) {
            output[i] = output[i] + bias2[i];
        }
        
        return output;
    }
};

// Scientific computation: Newton's method for root finding
float newton_method(float x0, int iterations) {
    float x = x0;
    for (int i = 0; i < iterations; ++i) {
        // f(x) = x^2 - 2 (find sqrt(2))
        // f'(x) = 2x
        x = x - (x * x - 2.0f) / (2.0f * x);
    }
    return x;
}

// Data processing: Matrix statistics
struct MatrixStats {
    float min_val;
    float max_val;
    float mean_val;
    float stdev;
};

MatrixStats compute_stats(const csb::Tensor<float>& matrix) {
    float min_val = matrix.min();
    float max_val = matrix.max();
    float mean_val = matrix.mean();
    
    // Compute standard deviation
    float sum_sq_diff = 0.0f;
    for (std::size_t i = 0; i < matrix.size(); ++i) {
        float diff = matrix[i] - mean_val;
        sum_sq_diff += diff * diff;
    }
    float stdev = std::sqrt(sum_sq_diff / matrix.size());

    return {min_val, max_val, mean_val, stdev};
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║       CODESPACE BACKEND - PRACTICAL DEMO APPLICATION            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

    // ============================================================
    // DEMO 1: Simple Neural Network Inference
    // ============================================================
    std::cout << "📊 DEMO 1: Neural Network Inference Pipeline\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    SimpleNN model;
    
    // Simulate MNIST image (784 = 28x28 pixel flatten)
    csb::Tensor<float> image({1, 784});
    image.fill(0.5f);  // Gray pixel values
    
    std::cout << "Input shape: [1, 784] (MNIST image)\n";
    std::cout << "Hidden layer: 128 neurons\n";
    std::cout << "Output: 10 classes (digits 0-9)\n\n";

    auto start = std::chrono::high_resolution_clock::now();
    auto predictions = model.forward(image);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Inference time: " << std::fixed << std::setprecision(3) << elapsed.count() << " ms\n";
    std::cout << "Output shape: [1, 10]\n";
    std::cout << "Predictions (logits): [ ";
    for (std::size_t i = 0; i < std::min(std::size_t(5), predictions.size()); ++i) {
        std::cout << std::setprecision(2) << predictions[i] << " ";
    }
    std::cout << "... ]\n\n";

    // ============================================================
    // DEMO 2: Scientific Computing - Root Finding
    // ============================================================
    std::cout << "🔬 DEMO 2: Scientific Computing - Root Finding\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    std::cout << "Problem: Find sqrt(2) using Newton's method\n";
    std::cout << "Equation: x^2 - 2 = 0\n\n";

    start = std::chrono::high_resolution_clock::now();
    float result = newton_method(1.5f, 100);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;

    std::cout << "Result: " << std::setprecision(10) << result << "\n";
    std::cout << "Actual sqrt(2): " << std::sqrt(2.0f) << "\n";
    std::cout << "Error: " << std::abs(result - std::sqrt(2.0f)) << "\n";
    std::cout << "Compute time: " << std::fixed << std::setprecision(3) << elapsed.count() << " ms\n\n";

    // ============================================================
    // DEMO 3: Data Processing & Statistics
    // ============================================================
    std::cout << "📈 DEMO 3: Data Processing & Statistics\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    // Simulate a dataset: 1000x100 (1000 samples, 100 features)
    csb::Tensor<float> dataset({1000, 100});
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(0.0f, 100.0f);
    
    for (std::size_t i = 0; i < dataset.size(); ++i) {
        dataset[i] = dist(rng);
    }

    std::cout << "Dataset shape: [1000, 100]\n";
    std::cout << "Total elements: " << dataset.size() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    auto stats = compute_stats(dataset);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;

    std::cout << "Statistics computed:\n";
    std::cout << "  Min value:     " << std::setprecision(4) << stats.min_val << "\n";
    std::cout << "  Max value:     " << stats.max_val << "\n";
    std::cout << "  Mean value:    " << stats.mean_val << "\n";
    std::cout << "  Std Dev:       " << stats.stdev << "\n";
    std::cout << "Compute time: " << std::fixed << std::setprecision(3) << elapsed.count() << " ms\n\n";

    // ============================================================
    // DEMO 4: Linear Algebra - Covariance Matrix Computation
    // ============================================================
    std::cout << "🧮 DEMO 4: Linear Algebra - Covariance\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    // Compute X^T * X for covariance
    csb::Tensor<float> X({100, 50});  // 100 samples, 50 features
    X.fill(1.0f);

    std::cout << "Data matrix shape: [100, 50]\n";
    std::cout << "Computing X^T * X (covariance basis)...\n\n";

    start = std::chrono::high_resolution_clock::now();
    auto X_T = X.transpose();
    auto cov = X_T.matmul(X);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;

    std::cout << "Result shape: [" << cov.shape()[0] << ", " << cov.shape()[1] << "]\n";
    std::cout << "Covariance[0,0]: " << cov(0, 0) << "\n";
    std::cout << "Compute time: " << std::fixed << std::setprecision(3) << elapsed.count() << " ms\n\n";

    // ============================================================
    // DEMO 5: Batch Processing - Tensor Operations
    // ============================================================
    std::cout << "⚡ DEMO 5: Batch Processing - Vectorized Operations\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    csb::Tensor<float> batch_a({512, 512});
    csb::Tensor<float> batch_b({512, 512});
    csb::Tensor<float> batch_c({512, 512});
    
    batch_a.fill(2.0f);
    batch_b.fill(3.0f);
    batch_c.fill(0.5f);

    std::cout << "Batch operations on 512x512 matrices:\n\n";

    // Operation 1: Addition
    start = std::chrono::high_resolution_clock::now();
    auto sum = batch_a.add(batch_b);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "ADD:        " << std::setw(8) << std::fixed << std::setprecision(2) 
              << (end - start).count() * 1000 << " µs  (result[0,0] = " << sum[0] << ")\n";

    // Operation 2: Multiplication
    start = std::chrono::high_resolution_clock::now();
    auto prod = batch_a.mul(batch_b);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "MUL:        " << std::setw(8) << std::fixed << std::setprecision(2) 
              << (end - start).count() * 1000 << " µs  (result[0,0] = " << prod[0] << ")\n";

    // Operation 3: Division
    start = std::chrono::high_resolution_clock::now();
    auto quot = batch_b.div(batch_c);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "DIV:        " << std::setw(8) << std::fixed << std::setprecision(2) 
              << (end - start).count() * 1000 << " µs  (result[0,0] = " << quot[0] << ")\n\n";

    // ============================================================
    // DEMO 6: Performance Scaling
    // ============================================================
    std::cout << "📊 DEMO 6: Performance Scaling Analysis\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    std::cout << "Matrix multiplication scaling (A*B = C, shapes [NxK]*[KxM] = [NxM]):\n\n";
    std::cout << std::left << std::setw(20) << "Dimensions" 
              << std::setw(15) << "Time (ms)" 
              << std::setw(15) << "GFLOPS\n";
    std::cout << std::string(50, '-') << "\n";

    for (int dim = 64; dim <= 512; dim *= 2) {
        csb::Tensor<float> A({dim, dim});
        csb::Tensor<float> B({dim, dim});
        A.fill(1.0f);
        B.fill(1.0f);

        start = std::chrono::high_resolution_clock::now();
        auto result = A.matmul(B);
        end = std::chrono::high_resolution_clock::now();
        
        double time_ms = (end - start).count() * 1000;
        double ops = 2.0 * dim * dim * dim;
        double gflops = ops / ((end - start).count() * 1e9);

        std::cout << std::setw(20) << (std::to_string(dim) + "x" + std::to_string(dim))
                  << std::setw(15) << std::fixed << std::setprecision(3) << time_ms
                  << std::setw(15) << std::fixed << std::setprecision(2) << gflops << "\n";
    }

    std::cout << "\n";

    // ============================================================
    // SUMMARY
    // ============================================================
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                          SUMMARY                               ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

    std::cout << "✅ Demonstrated Features:\n";
    std::cout << "   • Neural Network inference (matmul + element-wise ops)\n";
    std::cout << "   • Scientific computing (numerical algorithms)\n";
    std::cout << "   • Statistical analysis (descriptive statistics)\n";
    std::cout << "   • Linear algebra (matrix operations, transpose)\n";
    std::cout << "   • Batch processing (vectorized operations)\n";
    std::cout << "   • Performance scaling (GFLOPS measurement)\n\n";

    std::cout << "🎯 Use Cases:\n";
    std::cout << "   • CPU-bound ML inference servers\n";
    std::cout << "   • Scientific computing workloads\n";
    std::cout << "   • High-performance data processing\n";
    std::cout << "   • Embedded/edge AI systems\n";
    std::cout << "   • SIMD-optimized algorithms\n\n";

    std::cout << "⚡ Performance:\n";
    std::cout << "   • SIMD-accelerated operations (AVX2)\n";
    std::cout << "   • OpenMP-parallelized matrix multiplication\n";
    std::cout << "   • Aligned memory allocation for cache efficiency\n\n";

    std::cout << "📦 Next Steps:\n";
    std::cout << "   • Add broadcasting for flexible tensor shapes\n";
    std::cout << "   • Implement advanced indexing/slicing\n";
    std::cout << "   • Add automatic differentiation for training\n";
    std::cout << "   • GPU backend (ROCm) for AMD accelerators\n\n";

    return 0;
}
