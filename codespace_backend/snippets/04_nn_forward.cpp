// Quick Snippet: Neural Network Forward Pass
// Compile: g++ -O3 -march=native -std=c++23 -I../include -fopenmp nn_forward.cpp -o nn_forward && ./nn_forward

#include <iostream>
#include "csb/tensor.hpp"

class SimpleMLP {
public:
    csb::Tensor<float> w1, w2;
    csb::Tensor<float> b1, b2;

    SimpleMLP() : w1({784, 128}), w2({128, 10}), b1({128}), b2({10}) {
        w1.fill(0.01f);
        w2.fill(0.01f);
        b1.fill(0.0f);
        b2.fill(0.0f);
    }

    csb::Tensor<float> forward(const csb::Tensor<float>& x) {
        // x: [batch, 784]
        auto h = x.matmul(w1);  // [batch, 128]
        
        // Add bias (manual, no broadcasting yet)
        for (size_t i = 0; i < h.size(); ++i) {
            h[i] += b1[i % b1.size()];
        }
        
        // ReLU approximation
        for (size_t i = 0; i < h.size(); ++i) {
            if (h[i] < 0.0f) h[i] = 0.0f;
        }
        
        // Output layer
        auto out = h.matmul(w2);  // [batch, 10]
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] += b2[i % b2.size()];
        }
        
        return out;
    }
};

int main() {
    std::cout << "Neural Network Forward Pass\n";
    std::cout << "============================\n\n";

    SimpleMLP model;
    
    // Simulate batch of 32 MNIST images
    csb::Tensor<float> input({32, 784});
    input.fill(0.5f);  // Normalize pixel values
    
    std::cout << "Input shape:  [32, 784]\n";
    std::cout << "Hidden size:  128\n";
    std::cout << "Output size:  10\n\n";

    auto output = model.forward(input);
    
    std::cout << "Output shape: [" << output.shape()[0] << ", " << output.shape()[1] << "]\n";
    std::cout << "First batch prediction: [";
    for (size_t i = 0; i < 10; ++i) {
        std::cout << output(0, i) << (i < 9 ? ", " : "");
    }
    std::cout << "]\n";

    return 0;
}
