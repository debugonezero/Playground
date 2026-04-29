#ifndef CODESPACE_BACKEND_ARRAY_HPP
#define CODESPACE_BACKEND_ARRAY_HPP

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <immintrin.h>

namespace csb {

template<typename T>
class Array {
public:
    explicit Array(std::size_t size)
        : size_(size), data_(allocate(size))
    {
        if (!data_) {
            throw std::bad_alloc();
        }
    }

    Array(const Array& other)
        : size_(other.size_), data_(allocate(other.size_))
    {
        if (!data_) {
            throw std::bad_alloc();
        }
        std::memcpy(data_, other.data_, size_ * sizeof(T));
    }

    Array(Array&& other) noexcept
        : size_(other.size_), data_(other.data_)
    {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            reset();
            size_ = other.size_;
            data_ = allocate(size_);
            if (!data_) {
                throw std::bad_alloc();
            }
            std::memcpy(data_, other.data_, size_ * sizeof(T));
        }
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            reset();
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    ~Array() {
        reset();
    }

    T& operator[](std::size_t index) {
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        return data_[index];
    }

    std::size_t size() const noexcept {
        return size_;
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

    Array add(const Array& other) const {
        if (other.size_ != size_) {
            throw std::invalid_argument("array sizes must match");
        }
        Array result(size_);
        add_elements(data_, other.data_, result.data_, size_);
        return result;
    }

    void add_inplace(const Array& other) {
        if (other.size_ != size_) {
            throw std::invalid_argument("array sizes must match");
        }
        add_elements(data_, other.data_, data_, size_);
    }

private:
    std::size_t size_;
    T* data_;

    static T* allocate(std::size_t size) {
        if (size == 0) {
            return nullptr;
        }
        void* ptr = nullptr;
        const std::size_t alignment = 64;
        if (posix_memalign(&ptr, alignment, size * sizeof(T)) != 0) {
            return nullptr;
        }
        return static_cast<T*>(ptr);
    }

    void reset() noexcept {
        std::free(data_);
        data_ = nullptr;
        size_ = 0;
    }

    static void add_elements(const T* a, const T* b, T* out, std::size_t size) {
        if constexpr (std::is_same_v<T, float>) {
            std::size_t i = 0;
            const std::size_t step = 8;
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
            const std::size_t step = 4;
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
};

} // namespace csb

#endif // CODESPACE_BACKEND_ARRAY_HPP
