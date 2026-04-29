#ifndef CSB_TENSOR_HPP
#define CSB_TENSOR_HPP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <immintrin.h>
#include <omp.h>

namespace csb {

using Shape = std::vector<std::size_t>;

inline std::size_t compute_size(const Shape& shape) {
    std::size_t size = 1;
    for (auto dim : shape) {
        if (dim == 0) {
            return 0;
        }
        size *= dim;
    }
    return size;
}

inline Shape compute_strides(const Shape& shape) {
    Shape strides(shape.size());
    std::size_t stride = 1;
    for (std::size_t i = shape.size(); i-- > 0;) {
        strides[i] = stride;
        stride *= shape[i];
    }
    return strides;
}

inline std::size_t flatten_index(const Shape& strides, const Shape& indices) {
    if (strides.size() != indices.size()) {
        throw std::invalid_argument("index rank mismatch");
    }
    std::size_t flat = 0;
    for (std::size_t i = 0; i < strides.size(); ++i) {
        flat += strides[i] * indices[i];
    }
    return flat;
}

inline void require_same_shape(const Shape& a, const Shape& b) {
    if (a != b) {
        throw std::invalid_argument("tensor shapes must match");
    }
}

template<typename T>
class Tensor {
    static_assert(std::is_arithmetic_v<T>, "Tensor values must be arithmetic");

public:
    Tensor() noexcept = default;

    explicit Tensor(const Shape& shape)
        : shape_(shape), strides_(compute_strides(shape_)), size_(compute_size(shape_)), data_(allocate(size_))
    {
        if (size_ > 0 && !data_) {
            throw std::bad_alloc();
        }
    }

    Tensor(std::initializer_list<std::size_t> shape)
        : Tensor(Shape(shape))
    {}

    Tensor(const Tensor& other)
        : shape_(other.shape_), strides_(other.strides_), size_(other.size_), data_(allocate(other.size_))
    {
        if (size_ > 0 && !data_) {
            throw std::bad_alloc();
        }
        std::memcpy(data_, other.data_, size_ * sizeof(T));
    }

    Tensor(Tensor&& other) noexcept
        : shape_(std::move(other.shape_)), strides_(std::move(other.strides_)), size_(other.size_), data_(other.data_)
    {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    Tensor& operator=(const Tensor& other) {
        if (this != &other) {
            Tensor copy(other);
            swap(copy);
        }
        return *this;
    }

    Tensor& operator=(Tensor&& other) noexcept {
        if (this != &other) {
            reset();
            shape_ = std::move(other.shape_);
            strides_ = std::move(other.strides_);
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    ~Tensor() {
        reset();
    }

    T& operator[](std::size_t index) {
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        return data_[index];
    }

    template<typename... Indices>
    T& operator()(Indices... indices) {
        static_assert(sizeof...(Indices) > 0, "indices required");
        Shape idx{static_cast<std::size_t>(indices)...};
        return data_[flatten_index(strides_, idx)];
    }

    template<typename... Indices>
    const T& operator()(Indices... indices) const {
        static_assert(sizeof...(Indices) > 0, "indices required");
        Shape idx{static_cast<std::size_t>(indices)...};
        return data_[flatten_index(strides_, idx)];
    }

    std::size_t size() const noexcept {
        return size_;
    }

    const Shape& shape() const noexcept {
        return shape_;
    }

    const Shape& strides() const noexcept {
        return strides_;
    }

    T* data() noexcept {
        return data_;
    }

    const T* data() const noexcept {
        return data_;
    }

    void fill(T value) noexcept {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }

    void reshape(const Shape& shape) {
        if (compute_size(shape) != size_) {
            throw std::invalid_argument("reshape size mismatch");
        }
        shape_ = shape;
        strides_ = compute_strides(shape_);
    }

    Tensor transpose() const {
        if (shape_.size() != 2) {
            throw std::invalid_argument("transpose requires 2D tensor");
        }
        const std::size_t m = shape_[0];
        const std::size_t n = shape_[1];
        Tensor result({n, m});
        for (std::size_t i = 0; i < m; ++i) {
            for (std::size_t j = 0; j < n; ++j) {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    Tensor add(const Tensor& other) const {
        require_same_shape(shape_, other.shape_);
        Tensor result(shape_);
        add_impl(data_, other.data_, result.data_, size_);
        return result;
    }

    Tensor sub(const Tensor& other) const {
        require_same_shape(shape_, other.shape_);
        Tensor result(shape_);
        sub_impl(data_, other.data_, result.data_, size_);
        return result;
    }

    Tensor mul(const Tensor& other) const {
        require_same_shape(shape_, other.shape_);
        Tensor result(shape_);
        mul_impl(data_, other.data_, result.data_, size_);
        return result;
    }

    Tensor div(const Tensor& other) const {
        require_same_shape(shape_, other.shape_);
        Tensor result(shape_);
        div_impl(data_, other.data_, result.data_, size_);
        return result;
    }

    T sum() const noexcept {
        T total = T{};
        for (std::size_t i = 0; i < size_; ++i) {
            total += data_[i];
        }
        return total;
    }

    T min() const noexcept {
        if (size_ == 0) {
            return T{};
        }
        T result = data_[0];
        for (std::size_t i = 1; i < size_; ++i) {
            if (data_[i] < result) {
                result = data_[i];
            }
        }
        return result;
    }

    T max() const noexcept {
        if (size_ == 0) {
            return T{};
        }
        T result = data_[0];
        for (std::size_t i = 1; i < size_; ++i) {
            if (data_[i] > result) {
                result = data_[i];
            }
        }
        return result;
    }

    T mean() const noexcept {
        if (size_ == 0) {
            return T{};
        }
        return sum() / static_cast<T>(size_);
    }

    Tensor matmul(const Tensor& other) const {
        if (shape_.size() != 2 || other.shape_.size() != 2) {
            throw std::invalid_argument("matmul requires 2D tensors");
        }
        const std::size_t m = shape_[0];
        const std::size_t k = shape_[1];
        const std::size_t p = other.shape_[1];
        if (other.shape_[0] != k) {
            throw std::invalid_argument("matmul inner dimensions must match");
        }

        Tensor result({m, p});
        const T* a_data = data_;
        const T* b_data = other.data_;
        T* c_data = result.data_;

        #pragma omp parallel for collapse(2)
        for (std::size_t i = 0; i < m; ++i) {
            for (std::size_t j = 0; j < p; ++j) {
                T value = T{};
                for (std::size_t t = 0; t < k; ++t) {
                    value += a_data[i * k + t] * b_data[t * p + j];
                }
                c_data[i * p + j] = value;
            }
        }
        return result;
    }

private:
    Shape shape_;
    Shape strides_;
    std::size_t size_ = 0;
    T* data_ = nullptr;

    static T* allocate(std::size_t size) {
        if (size == 0) {
            return nullptr;
        }
        void* ptr = nullptr;
        constexpr std::size_t alignment = 64;
        if (posix_memalign(&ptr, alignment, size * sizeof(T)) != 0) {
            return nullptr;
        }
        return static_cast<T*>(ptr);
    }

    void reset() noexcept {
        std::free(data_);
        data_ = nullptr;
        size_ = 0;
        shape_.clear();
        strides_.clear();
    }

    void swap(Tensor& other) noexcept {
        std::swap(shape_, other.shape_);
        std::swap(strides_, other.strides_);
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }

    static void add_impl(const T* a, const T* b, T* out, std::size_t size) {
        if constexpr (std::is_same_v<T, float>) {
            std::size_t i = 0;
            constexpr std::size_t step = 8;
            for (; i + step <= size; i += step) {
                __m256 va = _mm256_load_ps(a + i);
                __m256 vb = _mm256_load_ps(b + i);
                __m256 vr = _mm256_add_ps(va, vb);
                _mm256_store_ps(out + i, vr);
            }
            for (; i < size; ++i) {
                out[i] = a[i] + b[i];
            }
        } else if constexpr (std::is_same_v<T, double>) {
            std::size_t i = 0;
            constexpr std::size_t step = 4;
            for (; i + step <= size; i += step) {
                __m256d va = _mm256_load_pd(a + i);
                __m256d vb = _mm256_load_pd(b + i);
                __m256d vr = _mm256_add_pd(va, vb);
                _mm256_store_pd(out + i, vr);
            }
            for (; i < size; ++i) {
                out[i] = a[i] + b[i];
            }
        } else {
            for (std::size_t i = 0; i < size; ++i) {
                out[i] = a[i] + b[i];
            }
        }
    }

    static void sub_impl(const T* a, const T* b, T* out, std::size_t size) {
        if constexpr (std::is_same_v<T, float>) {
            std::size_t i = 0;
            constexpr std::size_t step = 8;
            for (; i + step <= size; i += step) {
                __m256 va = _mm256_load_ps(a + i);
                __m256 vb = _mm256_load_ps(b + i);
                __m256 vr = _mm256_sub_ps(va, vb);
                _mm256_store_ps(out + i, vr);
            }
            for (; i < size; ++i) {
                out[i] = a[i] - b[i];
            }
        } else if constexpr (std::is_same_v<T, double>) {
            std::size_t i = 0;
            constexpr std::size_t step = 4;
            for (; i + step <= size; i += step) {
                __m256d va = _mm256_load_pd(a + i);
                __m256d vb = _mm256_load_pd(b + i);
                __m256d vr = _mm256_sub_pd(va, vb);
                _mm256_store_pd(out + i, vr);
            }
            for (; i < size; ++i) {
                out[i] = a[i] - b[i];
            }
        } else {
            for (std::size_t i = 0; i < size; ++i) {
                out[i] = a[i] - b[i];
            }
        }
    }

    static void mul_impl(const T* a, const T* b, T* out, std::size_t size) {
        if constexpr (std::is_same_v<T, float>) {
            std::size_t i = 0;
            constexpr std::size_t step = 8;
            for (; i + step <= size; i += step) {
                __m256 va = _mm256_load_ps(a + i);
                __m256 vb = _mm256_load_ps(b + i);
                __m256 vr = _mm256_mul_ps(va, vb);
                _mm256_store_ps(out + i, vr);
            }
            for (; i < size; ++i) {
                out[i] = a[i] * b[i];
            }
        } else if constexpr (std::is_same_v<T, double>) {
            std::size_t i = 0;
            constexpr std::size_t step = 4;
            for (; i + step <= size; i += step) {
                __m256d va = _mm256_load_pd(a + i);
                __m256d vb = _mm256_load_pd(b + i);
                __m256d vr = _mm256_mul_pd(va, vb);
                _mm256_store_pd(out + i, vr);
            }
            for (; i < size; ++i) {
                out[i] = a[i] * b[i];
            }
        } else {
            for (std::size_t i = 0; i < size; ++i) {
                out[i] = a[i] * b[i];
            }
        }
    }

    static void div_impl(const T* a, const T* b, T* out, std::size_t size) {
        if constexpr (std::is_same_v<T, float>) {
            std::size_t i = 0;
            constexpr std::size_t step = 8;
            for (; i + step <= size; i += step) {
                __m256 va = _mm256_load_ps(a + i);
                __m256 vb = _mm256_load_ps(b + i);
                __m256 vr = _mm256_div_ps(va, vb);
                _mm256_store_ps(out + i, vr);
            }
            for (; i < size; ++i) {
                out[i] = a[i] / b[i];
            }
        } else if constexpr (std::is_same_v<T, double>) {
            std::size_t i = 0;
            constexpr std::size_t step = 4;
            for (; i + step <= size; i += step) {
                __m256d va = _mm256_load_pd(a + i);
                __m256d vb = _mm256_load_pd(b + i);
                __m256d vr = _mm256_div_pd(va, vb);
                _mm256_store_pd(out + i, vr);
            }
            for (; i < size; ++i) {
                out[i] = a[i] / b[i];
            }
        } else {
            for (std::size_t i = 0; i < size; ++i) {
                out[i] = a[i] / b[i];
            }
        }
    }
};

} // namespace csb

#endif // CSB_TENSOR_HPP
