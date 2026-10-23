// clang++ -std=c++20 -fno-elide-constructors 3.01_function_calls.cpp

#include <utility>
#include <cstdint>
#include <new>
#include <iostream>

// --- Utility: stores a callable to run when the scope ends ---

template <typename F>
struct ScopeExit {
    F mF;
    ScopeExit(F f) : mF(f) {}
    ~ScopeExit() { mF(); }

    ScopeExit(const ScopeExit&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;
};

// --- Library Types ---

struct IntBox {
    IntBox(int x) : mData{new int(x)} {
        std::cout << "constructor" << std::endl;
    }
    ~IntBox() { 
        delete mData; 
        std::cout << "destructor" << std::endl;
    }

    IntBox(const IntBox& other) : mData{new int(*(other.mData))} {
        std::cout << "copy constructor" << std::endl;
    };
    IntBox& operator=(const IntBox& other) = delete;
    IntBox(IntBox&& other) : mData{std::exchange(other.mData, nullptr)} {
        std::cout << "move constructor" << std::endl;
    }
    IntBox& operator=(IntBox&& other) = delete;

    // returns a reference to the data contained
    int& get() {
        return *mData;
    }

    int *mData;
};

// --- Library Functions ---

IntBox getThree() {
    // construct `a` in this stack frame
    auto a = IntBox{3};
    // move construct `a` into the return slot
    return a;
}

// RVO does not occur
// Prevent NVRO with -fno-elide-constructors flag
IntBox product(IntBox& a, IntBox& b) {
    auto result = *(a.mData) * *(b.mData);
    auto retval = IntBox{result};
    return retval;
}

// --- Application Code ---

int demo() {
    auto a = IntBox{5};
    auto b = IntBox{6};

    // creates a temporary that is destroyed when expression ends
    std::cout << product(a, b).get() << std::endl;

    // equivalent to auto productResult = IntBox{product(a, b)};
    auto productResult = product(a, b);

    // Do whatever we want with productResult
    std::cout << "Modify product result" << std::endl;
    productResult.get() = 42;
    std::cout << productResult.get() << std::endl;

    return 0;
}

int demoExpanded() {
    auto a = IntBox{5};
    auto b = IntBox{6};

    /*
        std::cout << product(a, b).get();
    */
    // BEGIN EXPRESSION
    alignas(IntBox) uint8_t _tmp1[sizeof(IntBox)];
    auto _tmpIntBox1 = reinterpret_cast<IntBox *>(_tmp1);
    {   auto* _returnSlot = _tmpIntBox1; 
        auto& argA = a; auto& argB = b; 
    {
        auto result = *(argA.mData) * *(argB.mData);
        auto retval = IntBox{result};

        // use retval to construct into return slot
        new (_returnSlot) IntBox(std::move(retval));
    }}
    std::cout << _tmpIntBox1->get() << std::endl;
    _tmpIntBox1->~IntBox();
    // END EXPRESSION


    /*
        auto productResult = product(a, b); 
    */
    // BEGIN EXPRESSION
    alignas(IntBox) uint8_t _tmp2[sizeof(IntBox)];
    auto _tmpIntBox2 = reinterpret_cast<IntBox *>(_tmp2);
    {   auto* _returnSlot = _tmpIntBox2;
        auto& argA = a; auto& argB = b; 
    {
        auto result = *(argA.mData) * *(argB.mData);
        auto retval = IntBox{result};

        // use retval to construct into return slot
        new (_returnSlot) IntBox(std::move(retval));
    }}
    auto productResult = IntBox{std::move(*_tmpIntBox2)}; // slightly inaccurate
    _tmpIntBox2->~IntBox();
    // END EXPRESSION

    // Do whatever we want with productResult
    std::cout << "Modify product result" << std::endl;
    productResult.get() = 42;
    std::cout << productResult.get() << std::endl;

    return 0;
}

int main() {
    std::cout << "DEMO:" << std::endl;
    demo();

    std::cout << "DEMO EXPANDED:" << std::endl;
    demoExpanded();
}