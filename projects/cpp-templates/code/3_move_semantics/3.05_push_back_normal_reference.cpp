
// TODO: ask why data_[size_] = obj; asignment might be invalid (if not trivially constructible, nothing lives there)
// TODO: how can this interface be improved? obj is copied into array, then destroyed
// instead, it can be moved, and then destroyed
#include <iostream>
#include <utility>
#include <cassert>

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
    void push_back(T& obj) {
        assert(size_ < Capacity);
        // nothing is stopping you from writing this instead:
        // - new (data_ + size_) T(std::move(obj));
        // users of your library will just be confused:
        // - passing this argument as an rvalue is conventionally the same as giving permission for it to be moved-from
        // - users of your library expect that if they pass lvalues in, they won't become moved from 
        new (data_ + size_) T(obj);
        size_++;
    }

    void push_back(T&& obj) {
        assert(size_ < Capacity);
        new (data_ + size_) T(std::move(obj));
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

struct intbox {
    intbox(int x) : data_{new int(x)} {
        std::cout << "  constructor(" << *data_ << ")" << std::endl;
    }
    ~intbox() {
        if (data_) {
            std::cout << "  destructor(" << *data_ << ")" << std::endl;
        } else {
            std::cout << "  destructor(moved-from)" << std::endl;
        }
        delete data_;
    }

    intbox(const intbox& other) : data_{new int(*(other.data_))} {
        std::cout << "  copy constructor(" << *data_ << ")" << std::endl;
    }
    intbox& operator=(const intbox& other) = delete;
    intbox(intbox&& other) : data_{std::exchange(other.data_, nullptr)} {
        std::cout << "  move constructor(" << *data_ << ")" << std::endl;
    }
    intbox& operator=(intbox&& other) = delete;

    int& get() { return *data_; }
    int *data_;
};

int main() {


}