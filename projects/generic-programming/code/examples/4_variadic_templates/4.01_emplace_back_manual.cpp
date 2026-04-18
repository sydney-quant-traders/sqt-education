#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>
#include <utility>
#include <new>

// --- Library Code ---

template <typename T, std::size_t Capacity>
class bounded_vector {
public: // lifetime and resource management
    bounded_vector() : data_{static_cast<T*>(::operator new(sizeof(T) * Capacity))} {}
    ~bounded_vector() {
        if (data_) {
            for (std::size_t i = 0; i < size_; ++i) {
                data_[i].~T();
            }
            ::operator delete(data_);
        }
    }

    bounded_vector(const bounded_vector<T, Capacity>& other) 
    : data_{static_cast<T*>(::operator new(sizeof(T) * Capacity))}
    , size_{0}
    {
        std::size_t constructed = 0;
        try {
            for (std::size_t i = 0; i < other.size_; ++i) {
                new (data_ + i) T(other.data_[i]);
                ++constructed;
            }
            size_ = other.size_;
        } catch (std::exception& e) {
            for (std::size_t i = 0; i < constructed; ++i) {
                data_[i].~T();
            }
            // constructed object is now ready to be destroyed without leaks
            throw;
        }
    } 
    
    bounded_vector& operator=(const bounded_vector<T, Capacity>& other) {
        // invoke copy constructor
        auto copy = other;
        // invoke move assignment operator
        *this = std::move(copy);
        return *this;
    }

    bounded_vector(bounded_vector<T, Capacity>&& other) 
    : data_{std::exchange(other.data_, nullptr)} 
    , size_{std::exchange(other.size_, 0)}
    {}

    bounded_vector& operator=(bounded_vector<T, Capacity>&& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        return *this;
    }

public: // access
    T& at(std::size_t idx) {
        assert(idx < size_);
        return data_[idx];
    }

public: // modifiers

    template <typename F>
    void push_back(F&& obj) {
        assert(size_ < Capacity);
        new (data_ + size_) T(std::forward<F>(obj));
        size_++;
    }

    template <typename F1>
    void emplace_back(F1&& arg1) {
        assert(size_ < Capacity);
        new (data_ + size_) T(std::forward<F1>(arg1));
        size_++;
    }

    template <typename F1, typename F2>
    void emplace_back(F1&& arg1, F2&& arg2) {
        assert(size_ < Capacity);
        new (data_ + size_) T(std::forward<F1>(arg1), std::forward<F2>(arg2));
        size_++;
    }

    template <typename F1, typename F2, typename F3>
    void emplace_back(F1&& arg1, F2&& arg2, F3&& arg3) {
        assert(size_ < Capacity);
        new (data_ + size_) T(std::forward<F1>(arg1), std::forward<F2>(arg2), std::forward<F3>(arg3));
        size_++;
    }

    void pop_back() {
        assert(size_ != 0);
        size_--;
        data_[size_].~T();
    }

private:
    T *data_;
    std::size_t size_ = 0;

};

// --- Application Code ---

struct widget {
    widget(int& a, int b) {}
};

struct gadget {
    gadget(int a, int&& b, int&& c) {}
};

int main() {

    {
        bounded_vector<widget, 8> widget_vec;
        int x = 0;
        widget_vec.emplace_back(x, 0);
    }

    {
        bounded_vector<gadget, 8> gadget_vec;
        int x = 0;
        gadget_vec.emplace_back(0, std::move(x), 0);
    }
}
