# Codespace Backend - CPU Array Computing Library

A high-performance C++ tensor computing library optimized for AMD CPUs, designed for efficient array computations using SIMD intrinsics and OpenMP parallelism.

## Features

### Core Functionality
- **Multi-dimensional tensors** with shape and stride metadata
- **Type support**: `float`, `double`, `int32_t`, `int64_t`
- **64-byte aligned memory** for optimal cache performance
- **Contiguous storage** with efficient memory layout

### Operations

#### Elementwise Operations (SIMD-optimized for float/double)
- **Arithmetic**: `add()`, `sub()`, `mul()`, `div()`
- **Reductions**: `sum()`, `min()`, `max()`, `mean()`

#### Linear Algebra
- **Matrix Multiplication**: `matmul()` (OpenMP-parallelized)
- **Transpose**: `transpose()` for 2D tensors

#### Tensor Manipulation
- **Reshape**: `reshape()` to change dimensions
- **Indexing**: Multi-dimensional access via `operator()` or flat indexing via `operator[]`

### Performance Features
- **AVX2 SIMD kernels** for float/double operations
- **OpenMP parallelism** for matrix multiplication
- **-march=native -O3** compiler optimization
- **FMA (Fused Multiply-Add)** support for eligible CPU ISAs

## Build Instructions

### Prerequisites
- `cmake` >= 3.16
- `g++` >= 13.3 with C++23 support
- `OpenMP` development libraries (usually included with g++)

### Building

```bash
cd codespace_backend
./build.sh
```

This will:
1. Create a `build/` directory
2. Run `cmake` with optimization flags
3. Compile all executables: `codespace_example`, `codespace_test`, `codespace_benchmark`, `codespace_benchmark_all`

### Building Manually

```bash
cd codespace_backend
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j$(nproc)
```

## Usage

### Running Examples

```bash
./build/codespace_example       # Interactive demo of all features
./build/codespace_test          # Unit tests (9 test suites)
./build/codespace_benchmark     # Basic benchmark
./build/codespace_benchmark_all # Comprehensive benchmark suite
```

### C++ API

```cpp
#include "csb/tensor.hpp"

// Create tensors
csb::Tensor<float> a({256, 256});
csb::Tensor<float> b({256, 256});

// Fill with constant
a.fill(1.0f);
b.fill(2.0f);

// Arithmetic operations
auto c = a.add(b);
auto d = a.mul(b);

// Reductions
float sum_value = a.sum();
float min_value = a.min();
float max_value = a.max();
float mean_value = a.mean();

// Matrix multiplication
csb::Tensor<float> m1({128, 128});
csb::Tensor<float> m2({128, 128});
auto result = m1.matmul(m2);

// Transpose
auto m_t = m1.transpose();

// Access elements
float elem = a(10, 20);     // 2D indexing
float flat = a[5120];        // 1D flat indexing

// Reshape
a.reshape({512, 128});
```

## Architecture

```
codespace_backend/
├── include/csb/
│   └── tensor.hpp          # Main library header
├── examples/
│   └── main.cpp            # Usage examples
├── tests/
│   └── test_tensor.cpp     # Unit tests (9 test suites)
├── benchmarks/
│   ├── benchmark_add.cpp   # Basic operations benchmark
│   └── benchmark_all.cpp   # Comprehensive benchmark
├── CMakeLists.txt          # Build configuration
└── build.sh                # Helper build script
```

## Test Coverage

The test suite (`codespace_test`) includes:
1. **Float arithmetic** - add, sub, mul, div operations
2. **Reductions** - min, max, mean operations
3. **Transpose** - 2D matrix transposition
4. **Matrix multiplication** - basic matmul tests
5. **Reshape** - tensor shape changes
6. **Double precision** - SIMD kernels with double type
7. **int32_t support** - integer type arithmetic
8. **int64_t support** - large integer arithmetic
9. **Copy/move semantics** - memory management tests

## Benchmark Results

Example performance on AMD EPYC 7763 (2 vCPUs):

```
Float operations (size = 2^24 = 16M elements):
  ADD             0.001 s     12.8 GFLOPS
  SUB             0.001 s     12.9 GFLOPS
  MUL             0.001 s     13.2 GFLOPS
  DIV             0.011 s      1.5 GFLOPS

Matrix operations (square matrices 512x512):
  MATMUL (512x512) 0.845 s     0.32 GFLOPS

Transpose operations (512x512):
  TRANSPOSE (512x512) 0.002 s
```

## Data Types Support

| Type | SIMD Kernel | Notes |
|------|-------------|-------|
| `float` | AVX2 8-element | Full SIMD optimization |
| `double` | AVX2 4-element | Full SIMD optimization |
| `int32_t` | Loop only | No SIMD yet |
| `int64_t` | Loop only | No SIMD yet |

## Memory Layout

- **Alignment**: 64-byte aligned (cache-line aware)
- **Storage**: Row-major (C-style) contiguous allocation
- **Strides**: Computed from shape for efficient indexing

## Future Enhancements

- [ ] **Slicing/views** - non-owning tensor views
- [ ] **int32_t/int64_t SIMD** - integer SIMD kernels
- [ ] **Broadcasting** - automatic dimension expansion
- [ ] **Convolution** - 2D convolution operations
- [ ] **FFT** - Fast Fourier Transform
- [ ] **GPU support** - ROCm/HIP backend for AMD GPUs
- [ ] **Sparse tensors** - COO/CSR sparse formats

## Compiler Support

Requires C++23 with:
- `-march=native` for CPU-specific optimizations
- `-O3` for aggressive optimization
- `-fopenmp` for OpenMP parallelism
- AVX2 CPU support (present in AMD EPYC, Ryzen, etc.)

## Performance Tips

1. **Align data**: Always allocate through the library (64-byte alignment automatic)
2. **Use float when possible**: float is ~2x faster than double on SIMD ops
3. **Batch operations**: Process large tensors instead of small ones
4. **Reuse tensors**: Minimize allocation/deallocation overhead
5. **Enable OpenMP**: Set `OMP_NUM_THREADS` for matmul parallelism

## License

MIT License - see LICENSE file for details

## References

- [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/develop/documentation/cpp-compiler-developer-guide-and-reference/top/compiler-reference/intrinsics.html)
- [OpenMP Specifications](https://www.openmp.org/spec-html/5.0/openmpsu.html)
- [ANSI/IEEE Std 754-2019](https://ieeexplore.ieee.org/document/8766229) (Floating Point)
