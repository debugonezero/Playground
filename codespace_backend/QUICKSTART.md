# Quick C++ Development Guide - Codespace Backend

## ⚡ Ultra-Fast Setup (< 1 minute)

### 1. Create New Project
```bash
cd /workspaces/Playground/codespace_backend
chmod +x quickdev.sh
./quickdev.sh my_project
cd my_project
./build.sh
./build/main
```

### 2. One-File Project (Copy-Paste Ready)

Copy this, save as `tensor_script.cpp`, compile with:
```bash
g++ -O3 -march=native -std=c++23 -I/path/to/include tensor_script.cpp -o tensor_script -fopenmp
./tensor_script
```

**Minimal Template:**
```cpp
#include <iostream>
#include "csb/tensor.hpp"

int main() {
    csb::Tensor<float> a({100, 100});
    csb::Tensor<float> b({100, 100});
    
    a.fill(1.0f);
    b.fill(2.0f);
    
    auto c = a.add(b);
    std::cout << "Result: " << c[0] << "\n";
    
    return 0;
}
```

---

## 🚀 Common Code Patterns

### Pattern 1: Simple Array Operations
```cpp
#include "csb/tensor.hpp"

csb::Tensor<float> a({1000});
csb::Tensor<float> b({1000});

a.fill(2.5f);
b.fill(1.5f);

auto sum = a.add(b);      // Element-wise add
auto prod = a.mul(b);     // Element-wise multiply
auto ratio = a.div(b);    // Element-wise divide

float total = a.sum();    // Reduction
float min_val = a.min();  // Min over all elements
float max_val = a.max();  // Max over all elements
float avg = a.mean();     // Mean value
```

### Pattern 2: Matrix Operations
```cpp
#include "csb/tensor.hpp"

csb::Tensor<float> A({256, 256});
csb::Tensor<float> B({256, 256});

A.fill(1.0f);
B.fill(2.0f);

// Matrix multiplication
auto C = A.matmul(B);

// Transpose
auto A_T = A.transpose();

// Combine
auto result = A_T.matmul(B);
```

### Pattern 3: Multi-Dimensional Indexing
```cpp
#include "csb/tensor.hpp"

csb::Tensor<float> matrix({10, 20, 30});
matrix.fill(5.0f);

// 1D indexing (flat)
float val1 = matrix[0];
matrix[100] = 3.14f;

// Multi-D indexing (operator())
float val2 = matrix(2, 5, 10);
matrix(0, 0, 0) = 1.0f;

// Get shape and size
auto shape = matrix.shape();      // [10, 20, 30]
auto size = matrix.size();         // 6000
auto data_ptr = matrix.data();     // Raw pointer
```

### Pattern 4: Reshape & Manipulation
```cpp
#include "csb/tensor.hpp"

csb::Tensor<float> a({100, 100});  // 10,000 elements
a.fill(1.0f);

// Reshape to different dimensions
a.reshape({200, 50});              // Still 10,000 elements
a.reshape({10, 1000});

// Transpose (for 2D matrices)
csb::Tensor<float> mat({50, 100});
auto mat_T = mat.transpose();      // Now [100, 50]
```

### Pattern 5: Statistical Analysis
```cpp
#include "csb/tensor.hpp"

csb::Tensor<float> data({10000});
// ... populate data ...

// Compute statistics
float min_val = data.min();
float max_val = data.max();
float mean = data.mean();
float sum = data.sum();

// Manual std dev
float variance = 0.0f;
for (size_t i = 0; i < data.size(); ++i) {
    float diff = data[i] - mean;
    variance += diff * diff;
}
variance /= data.size();
float stdev = std::sqrt(variance);
```

### Pattern 6: Loop Over Tensor
```cpp
#include "csb/tensor.hpp"

csb::Tensor<float> t({100, 100});
t.fill(2.0f);

// Flat iteration
for (size_t i = 0; i < t.size(); ++i) {
    t[i] = t[i] * 2.0f;
}

// 2D iteration
auto shape = t.shape();
for (size_t i = 0; i < shape[0]; ++i) {
    for (size_t j = 0; j < shape[1]; ++j) {
        t(i, j) = t(i, j) + 1.0f;
    }
}
```

### Pattern 7: Timing Code
```cpp
#include <chrono>
#include "csb/tensor.hpp"

csb::Tensor<float> a({1000, 1000});
csb::Tensor<float> b({1000, 1000});
a.fill(1.0f);
b.fill(2.0f);

auto start = std::chrono::high_resolution_clock::now();
auto result = a.matmul(b);
auto end = std::chrono::high_resolution_clock::now();

double elapsed_ms = (end - start).count() * 1000.0;
std::cout << "Time: " << elapsed_ms << " ms\n";

// Or in microseconds
auto elapsed_us = (end - start).count() * 1e6;
std::cout << "Time: " << elapsed_us << " µs\n";
```

### Pattern 8: Type Flexibility
```cpp
#include "csb/tensor.hpp"

// Float precision
csb::Tensor<float> f32({100, 100});
f32.fill(1.5f);

// Double precision
csb::Tensor<double> f64({100, 100});
f64.fill(2.5);

// Integer types
csb::Tensor<int32_t> i32({50, 50});
i32.fill(42);

csb::Tensor<int64_t> i64({75, 75});
i64.fill(999);

// All support the same API
auto f_result = f32.add(f32);
auto i_result = i32.add(i32);
```

---

## 📋 Quick Reference - API Cheatsheet

```cpp
// Construction
csb::Tensor<T> t({dim1, dim2, ...});     // Create tensor
csb::Tensor<T> copy = original;           // Copy constructor
csb::Tensor<T> moved = std::move(other);  // Move constructor

// Access
t[i];                           // Flat indexing
t(i, j, k);                     // Multi-D indexing
t.data();                        // Raw pointer

// Info
t.size();                        // Total elements
t.shape();                       // Vector of dimensions
t.strides();                     // Vector of strides

// Fill & Modify
t.fill(value);                   // Fill all with value
t.reshape({new_dims});           // Change shape (must match size)

// Elementwise Ops (returns new tensor)
auto result = a.add(b);          // Element-wise addition
auto result = a.sub(b);          // Element-wise subtraction
auto result = a.mul(b);          // Element-wise multiply
auto result = a.div(b);          // Element-wise divide

// Reductions (return scalar)
float s = t.sum();               // Sum all elements
float m = t.min();               // Min value
float x = t.max();               // Max value
float avg = t.mean();            // Mean value

// Linear Algebra
auto C = A.matmul(B);            // Matrix multiplication
auto A_T = A.transpose();        // Transpose (2D only)

// Copy/Move
auto copy = original;            // Copy (full data copy)
auto moved = std::move(other);   // Move (takes ownership)
```

---

## 🔨 Fastest Compile Commands

### Single File (Standalone)
```bash
g++ -O3 -march=native -std=c++23 -I/path/to/backend/include \
    -fopenmp mycode.cpp -o mycode && ./mycode
```

### CMake Project (Recommended)
```bash
mkdir build && cd build
cmake .. && make -j$(nproc) && ./main
```

### Watch & Rebuild (Development Loop)
```bash
# Terminal 1: Watch for changes and rebuild
while true; do inotifywait -e modify src/*.cpp && \
  (cd build && make -j$(nproc) && echo "✅ Built") || echo "❌ Error"; done

# Terminal 2: Run program
./build/main
```

---

## 💡 Pro Tips for Fast Development

### 1. Use Compiler Flags for Speed
```bash
# Maximum optimization
g++ -O3 -march=native -flto ...

# Fast compilation (lower opt)
g++ -O2 -std=c++23 ...

# Debug with symbols
g++ -g -O0 -std=c++23 ...
```

### 2. Template Specialization (Optional)
```cpp
// Compiler will specialize at compile-time
csb::Tensor<float> fast_tensor;    // Optimized for float
csb::Tensor<double> precise_tensor; // Optimized for double
```

### 3. Move Semantics for Performance
```cpp
// Avoid copy overhead
auto result = expensive_op();      // Returns by value (uses move)
another_tensor = std::move(result); // No copy, just pointer swap
```

### 4. Batch Operations
```cpp
// Better: batch operation
for (int i = 0; i < 1000; ++i) {
    large_tensor.mul(scale_factor);  // Single SIMD operation
}

// Worse: small operations
for (int i = 0; i < 1000000; ++i) {
    small_val[i] = small_val[i] * 2; // Many small ops
```

### 5. In-Place Operations (When Needed)
```cpp
// Create result tensor first to avoid allocations
csb::Tensor<float> result = a.add(b);
// Later reuse result
result = c.mul(d);  // Reuse allocation
```

---

## 🎯 Project Templates

### Template A: Data Analysis
```cpp
#include <iostream>
#include <fstream>
#include "csb/tensor.hpp"

int main() {
    // Load data
    csb::Tensor<float> data({10000, 100});
    // ... read from file or generate ...
    
    // Compute statistics
    for (size_t col = 0; col < 100; ++col) {
        // Process column
    }
    
    // Output results
    std::cout << "Analysis complete\n";
    return 0;
}
```

### Template B: ML Inference
```cpp
#include "csb/tensor.hpp"

class Model {
public:
    csb::Tensor<float> W1, b1, W2, b2;
    
    csb::Tensor<float> forward(const csb::Tensor<float>& x) {
        auto h = x.matmul(W1);
        // Apply activation...
        return h.matmul(W2);
    }
};

int main() {
    Model m;
    csb::Tensor<float> input({1, 784});
    auto output = m.forward(input);
    return 0;
}
```

### Template C: Scientific Computation
```cpp
#include <cmath>
#include "csb/tensor.hpp"

int main() {
    csb::Tensor<float> A({1000, 1000});
    csb::Tensor<float> B({1000, 1000});
    
    A.fill(1.0f);
    B.fill(2.0f);
    
    auto C = A.matmul(B);
    auto C_mean = C.mean();
    
    std::cout << "Result: " << C_mean << "\n";
    return 0;
}
```

---

## ❓ Troubleshooting

| Problem | Solution |
|---------|----------|
| "tensor.hpp not found" | Add `-I/path/to/backend/include` |
| "Shape mismatch" error | Ensure operands have same shape |
| Slow compilation | Use `-O2` instead of `-O3` |
| Bad performance | Check `-march=native` flag |
| Link errors | Add `-fopenmp` for OpenMP |

---

## 📚 Resources

- **Header**: `/workspaces/Playground/codespace_backend/include/csb/tensor.hpp`
- **Examples**: `/workspaces/Playground/codespace_backend/examples/`
- **Tests**: `/workspaces/Playground/codespace_backend/tests/test_tensor.cpp`
- **Benchmarks**: `/workspaces/Playground/codespace_backend/benchmarks/`

---

## ⚡ Summary: The 30-Second Start

```bash
# 1. Create file
cat > quick.cpp << 'EOF'
#include "csb/tensor.hpp"
int main() {
    csb::Tensor<float> a({100, 100}), b({100, 100});
    a.fill(1.0f); b.fill(2.0f);
    auto c = a.matmul(b);
    std::cout << c[0] << "\n";
}
EOF

# 2. Compile & Run
g++ -O3 -march=native -std=c++23 -I/workspaces/Playground/codespace_backend/include \
    -fopenmp quick.cpp -o quick && ./quick
```

Done! 🚀
