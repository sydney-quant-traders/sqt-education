#include <iostream>
#include <utility>
#include <cassert>

// --- Library Code ---

template <typename T, std::size_t Capacity>
class BoundedVector 
{
    T *mData;
    std::size_t mSize = 0;

public: // lifetime and resource management
    BoundedVector() 
        : mData{static_cast<T*>(::operator new(sizeof(T) * Capacity))}
    {}

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
    // bad method design but we will deal with this later
    void pushBack(T obj) {
        assert(mSize < Capacity);
        new (mData + mSize) T(obj);
        mSize++;
    }

    void popBack() {
        assert(mSize != 0);
        mSize--;
        mData[mSize].~T();
    }
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
    std::cout << "--- pushBack 3 intboxes ---\n";
    auto v = BoundedVector<IntBox, 8>{};
    v.pushBack(IntBox(10));
    v.pushBack(IntBox(20));
    v.pushBack(IntBox(30));

    {
        std::cout << "\n--- copy construct v2 from v ---\n";
        auto v2 = v;
        std::cout << "\n--- v2 going out of scope ---\n";
    }

    {
        std::cout << "\n--- copy assign v3 = v ---\n";
        auto v3 = BoundedVector<IntBox, 8>{};
        v3.pushBack(IntBox(99));
        v3 = v;
        std::cout << "\n--- v3 going out of scope ---\n";
    }

    auto v4 = BoundedVector<IntBox, 8>{};
    {
        std::cout << "\n--- move construct from v ---\n";
        auto moved = std::move(v);

        std::cout << "\n--- move assign to v4 ---\n";
        v4 = std::move(moved);
        std::cout << "\n--- moved going out of scope (moved-from) ---\n";
    }

    std::cout << "\n--- reassign v = v4 (v is moved-from) ---\n";
    v = v4;

    std::cout << "\n--- end of main, v4 then v destroyed ---\n";
}
