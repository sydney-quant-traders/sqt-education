
// TODO: ask why mData[mSize] = obj; asignment might be invalid (if not trivially constructible, nothing lives there)
// TODO: how can this interface be improved? obj is copied into array, then destroyed
// instead, it can be moved, and then destroyed
#include <iostream>
#include <utility>
#include <cassert>

// --- Library Code ---

template <typename T, std::size_t Capacity>
class BoundedVector {
public: // lifetime and resource management
    BoundedVector() : mData{static_cast<T*>(::operator new(sizeof(T) * Capacity))} {}
    ~BoundedVector() {
        if (mData) {
            for (std::size_t i = 0; i < mSize; ++i) {
                mData[i].~T();
            }
            ::operator delete(mData);
        }
    }

    BoundedVector(const BoundedVector<T, Capacity>& other) 
    : mData{static_cast<T*>(::operator new(sizeof(T) * Capacity))}
    , mSize{0}
    {
        auto constructed = std::size_t{0};
        try {
            for (std::size_t i = 0; i < other.mSize; ++i) {
                new (mData + i) T(other.mData[i]);
                ++constructed;
            }
            mSize = other.mSize;
        } catch (std::exception& e) {
            for (std::size_t i = 0; i < constructed; ++i) {
                mData[i].~T();
            }
            // constructed object is now ready to be destroyed without leaks
            throw;
        }
    } 
    
    BoundedVector& operator=(const BoundedVector<T, Capacity>& other) {
        // invoke copy constructor
        auto copy = other;
        // invoke move assignment operator
        *this = std::move(copy);
        return *this;
    }

    BoundedVector(BoundedVector<T, Capacity>&& other) 
    : mData{std::exchange(other.mData, nullptr)} 
    , mSize{std::exchange(other.mSize, 0)}
    {}

    BoundedVector& operator=(BoundedVector<T, Capacity>&& other) noexcept {
        std::swap(mData, other.mData);
        std::swap(mSize, other.mSize);
        return *this;
    }

public: // access
    T& at(std::size_t idx) {
        assert(idx < mSize);
        return mData[idx];
    }

public: // modifiers

    template <typename F>
    void pushBack(F&& obj) {
        assert(mSize < Capacity);
        new (mData + mSize) T(std::forward<F>(obj));
        mSize++;
    }

    void popBack() {
        assert(mSize != 0);
        mSize--;
        mData[mSize].~T();
    }

private:
    T *mData;
    std::size_t mSize = 0;

};

// --- Application Code ---

struct IntBox {
    IntBox(int x) : mData{new int(x)} {
        std::cout << "  constructor(" << *mData << ")" << std::endl;
    }
    ~IntBox() {
        if (mData) {
            std::cout << "  destructor(" << *mData << ")" << std::endl;
        } else {
            std::cout << "  destructor(moved-from)" << std::endl;
        }
        delete mData;
    }

    IntBox(const IntBox& other) : mData{new int(*(other.mData))} {
        std::cout << "  copy constructor(" << *mData << ")" << std::endl;
    }
    IntBox& operator=(const IntBox& other) = delete;
    IntBox(IntBox&& other) : mData{std::exchange(other.mData, nullptr)} {
        std::cout << "  move constructor(" << *mData << ")" << std::endl;
    }
    IntBox& operator=(IntBox&& other) = delete;

    int& get() { return *mData; }
    int *mData;
};

int main() {

}
