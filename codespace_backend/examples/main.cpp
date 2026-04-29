#include <chrono>
#include <iostream>
#include "csb/tensor.hpp"

int main() {
    std::cout << "=== Codespace Backend Example ===\n\n";

    // Example 1: Float tensor operations
    std::cout << "Example 1: Float tensor operations\n";
    const csb::Shape shape = {256, 256};
    csb::Tensor<float> a(shape);
    csb::Tensor<float> b(shape);
    a.fill(1.0f);
    b.fill(2.0f);

    auto start = std::chrono::high_resolution_clock::now();
    auto c = a.add(b);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "  tensor add c(0,0) = " << c(0, 0) << "\n";
    std::cout << "  tensor sum = " << c.sum() << "\n";
    std::cout << "  add elapsed = " << elapsed.count() << " s\n";
    std::cout << "  tensor shape = [" << shape[0] << ", " << shape[1] << "]\n\n";

    // Example 2: Matrix multiplication
    std::cout << "Example 2: Matrix multiplication\n";
    auto x = csb::Tensor<float>({128, 128});
    auto y = csb::Tensor<float>({128, 128});
    x.fill(1.0f);
    y.fill(2.0f);

    start = std::chrono::high_resolution_clock::now();
    auto mat = x.matmul(y);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;

    std::cout << "  matmul result shape = [" << mat.shape()[0] << ", " << mat.shape()[1] << "]\n";
    std::cout << "  matmul sample = " << mat(0, 0) << "\n";
    std::cout << "  matmul elapsed = " << elapsed.count() << " s\n\n";

    // Example 3: Transpose
    std::cout << "Example 3: Transpose operation\n";
    csb::Tensor<float> m({3, 4});
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            m(i, j) = static_cast<float>(i * 4 + j + 1);
        }
    }
    std::cout << "  Original shape: [" << m.shape()[0] << ", " << m.shape()[1] << "]\n";
    auto m_t = m.transpose();
    std::cout << "  Transposed shape: [" << m_t.shape()[0] << ", " << m_t.shape()[1] << "]\n";
    std::cout << "  Original m(0,1) = " << m(0, 1) << ", Transposed m_t(1,0) = " << m_t(1, 0) << "\n\n";

    // Example 4: Reductions (min, max, mean)
    std::cout << "Example 4: Reduction operations\n";
    csb::Tensor<float> data({5});
    data[0] = 10.5f;
    data[1] = 3.2f;
    data[2] = 15.7f;
    data[3] = 5.1f;
    data[4] = 8.9f;
    std::cout << "  min = " << data.min() << "\n";
    std::cout << "  max = " << data.max() << "\n";
    std::cout << "  mean = " << data.mean() << "\n";
    std::cout << "  sum = " << data.sum() << "\n\n";

    // Example 5: Double precision
    std::cout << "Example 5: Double precision operations\n";
    csb::Tensor<double> d1({1000});
    csb::Tensor<double> d2({1000});
    d1.fill(1.5);
    d2.fill(2.5);
    auto d_result = d1.add(d2);
    std::cout << "  double add result: " << d_result[0] << "\n";
    std::cout << "  double mul result: " << d1.mul(d2)[0] << "\n\n";

    // Example 6: Integer types
    std::cout << "Example 6: Integer type support\n";
    csb::Tensor<int32_t> i32({4});
    i32[0] = 100;
    i32[1] = 200;
    i32[2] = 150;
    i32[3] = 250;
    std::cout << "  int32_t min = " << i32.min() << "\n";
    std::cout << "  int32_t max = " << i32.max() << "\n";
    std::cout << "  int32_t sum = " << i32.sum() << "\n";

    csb::Tensor<int64_t> i64({3, 3});
    i64.fill(999999999LL);
    std::cout << "  int64_t value: " << i64(0, 0) << "\n";
    std::cout << "  int64_t sum: " << i64.sum() << "\n\n";

    // Example 7: All elementwise operations
    std::cout << "Example 7: All elementwise operations\n";
    csb::Tensor<float> op1({3});
    csb::Tensor<float> op2({3});
    op1[0] = 10.0f; op1[1] = 20.0f; op1[2] = 30.0f;
    op2[0] = 2.0f;  op2[1] = 4.0f;  op2[2] = 5.0f;

    auto add_res = op1.add(op2);
    auto sub_res = op1.sub(op2);
    auto mul_res = op1.mul(op2);
    auto div_res = op1.div(op2);

    std::cout << "  ADD: " << add_res[0] << ", " << add_res[1] << ", " << add_res[2] << "\n";
    std::cout << "  SUB: " << sub_res[0] << ", " << sub_res[1] << ", " << sub_res[2] << "\n";
    std::cout << "  MUL: " << mul_res[0] << ", " << mul_res[1] << ", " << mul_res[2] << "\n";
    std::cout << "  DIV: " << div_res[0] << ", " << div_res[1] << ", " << div_res[2] << "\n\n";

    std::cout << "All examples completed successfully!\n";
    return 0;
}
