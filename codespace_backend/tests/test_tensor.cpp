#include <cassert>
#include <iostream>
#include <cmath>
#include "csb/tensor.hpp"

int main() {
    // Test 1: Basic arithmetic with float
    std::cout << "Test 1: Basic arithmetic with float\n";
    csb::Tensor<float> a({2, 3});
    csb::Tensor<float> b({2, 3});
    a.fill(1.0f);
    b.fill(2.0f);

    auto c = a.add(b);
    assert(c.shape() == a.shape());
    assert(c[0] == 3.0f);
    assert(c.sum() == 9.0f);

    auto d = a.sub(b);
    assert(d[0] == -1.0f);

    auto e = a.mul(b);
    assert(e[0] == 2.0f);

    auto f = b.div(a);
    assert(f[0] == 2.0f);
    std::cout << "  PASS\n";

    // Test 2: Reduce operations (min, max, mean)
    std::cout << "Test 2: Reduce operations\n";
    csb::Tensor<float> g({4});
    g[0] = 5.0f;
    g[1] = 2.0f;
    g[2] = 8.0f;
    g[3] = 3.0f;
    assert(g.min() == 2.0f);
    assert(g.max() == 8.0f);
    assert(std::abs(g.mean() - 4.5f) < 1e-6f);
    std::cout << "  PASS\n";

    // Test 3: Transpose
    std::cout << "Test 3: Transpose\n";
    csb::Tensor<float> x({2, 3});
    x(0, 0) = 1.0f;
    x(0, 1) = 2.0f;
    x(0, 2) = 3.0f;
    x(1, 0) = 4.0f;
    x(1, 1) = 5.0f;
    x(1, 2) = 6.0f;

    auto x_t = x.transpose();
    assert((x_t.shape() == csb::Shape{3, 2}));
    assert(x_t(0, 0) == 1.0f);
    assert(x_t(1, 0) == 2.0f);
    assert(x_t(2, 1) == 6.0f);
    std::cout << "  PASS\n";

    // Test 4: Matrix multiplication with float
    std::cout << "Test 4: Matrix multiplication (float)\n";
    csb::Tensor<float> m1({2, 3});
    csb::Tensor<float> m2({3, 2});
    m1.fill(1.0f);
    m2.fill(2.0f);

    auto mat = m1.matmul(m2);
    assert(((mat.shape() == csb::Shape{2, 2})));
    assert((mat(0, 0) == 6.0f));
    assert((mat(1, 1) == 6.0f));
    std::cout << "  PASS\n";

    // Test 5: Reshape
    std::cout << "Test 5: Reshape\n";
    csb::Tensor<float> p({4, 3});
    p.fill(5.0f);
    p.reshape({2, 6});
    assert((p.shape() == csb::Shape{2, 6}));
    assert(p[0] == 5.0f);
    assert(p.size() == 12);
    std::cout << "  PASS\n";

    // Test 6: Double type with SIMD
    std::cout << "Test 6: Double type with SIMD\n";
    csb::Tensor<double> d1({4});
    csb::Tensor<double> d2({4});
    d1.fill(1.5);
    d2.fill(2.5);
    auto d_add = d1.add(d2);
    assert(std::abs(d_add[0] - 4.0) < 1e-15);
    auto d_mul = d1.mul(d2);
    assert(std::abs(d_mul[0] - 3.75) < 1e-15);
    std::cout << "  PASS\n";

    // Test 7: int32_t type
    std::cout << "Test 7: int32_t type\n";
    csb::Tensor<int32_t> i32_a({3});
    csb::Tensor<int32_t> i32_b({3});
    i32_a[0] = 10;
    i32_a[1] = 20;
    i32_a[2] = 30;
    i32_b[0] = 5;
    i32_b[1] = 10;
    i32_b[2] = 15;
    auto i32_add = i32_a.add(i32_b);
    assert(i32_add[0] == 15);
    assert(i32_add[1] == 30);
    assert(i32_add.sum() == 75);
    assert((i32_a.min() == 10));
    assert((i32_a.max() == 30));
    std::cout << "  PASS\n";

    // Test 8: int64_t type
    std::cout << "Test 8: int64_t type\n";
    csb::Tensor<int64_t> i64_a({2, 2});
    i64_a(0, 0) = 100;
    i64_a(0, 1) = 200;
    i64_a(1, 0) = 300;
    i64_a(1, 1) = 400;
    assert(i64_a(0, 0) == 100);
    assert(i64_a.sum() == 1000);
    csb::Tensor<int64_t> i64_b({2, 2});
    i64_b.fill(1);
    auto i64_sub = i64_a.sub(i64_b);
    assert(i64_sub(0, 0) == 99);
    std::cout << "  PASS\n";

    // Test 9: Copy and move semantics
    std::cout << "Test 9: Copy and move semantics\n";
    csb::Tensor<float> orig({2});
    orig[0] = 3.14f;
    orig[1] = 2.71f;

    csb::Tensor<float> copy = orig;
    assert(copy[0] == 3.14f);
    assert(copy[1] == 2.71f);

    csb::Tensor<float> moved = std::move(orig);
    assert(moved[0] == 3.14f);
    assert(orig.size() == 0);
    std::cout << "  PASS\n";

    std::cout << "\nAll tests PASSED!\n";
    return 0;
}
