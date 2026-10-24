// clang++ -std=c++20 -fno-elide-constructors 3.01_function_calls_expanded.cpp

#include <utility>
#include <cstdint>
#include <new>
#include <iostream>

// --- Library Types ---

struct IntBox {
    int *mData;

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

    // returns a reference to the data contained
    int& get() {
        return *mData;
    }
};

// --- Library Functions ---

// RVO always occurs here
IntBox sum(IntBox& a, IntBox& b) {
    auto result = *(a.mData) + *(b.mData);
    return IntBox(result);
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

    // RVO, return slot points to sumResult storage
    auto sumResult = sum(a, b);

    // No RVO, return slot points to productResult storage
    // - however an extra move occurs in the function body
    auto productResult = product(a, b);

    // No RVO, return slot points to a temporary, not a named variable
    std::cout << product(a, b).get() << std::endl;

    return 0;
}

// demo() but with all function calls inlined, for educational purposes
// you would never write code like this
int demoExpanded() {
    auto a = IntBox{5};
    auto b = IntBox{6};

    /*
        auto sumResult = sum(a, static_cast<IntBox&&>(b));
    */
    // BEGIN EXPRESSION
    alignas(IntBox) uint8_t _storageSumResult[sizeof(IntBox)];
    {   auto* _returnSlot = reinterpret_cast<IntBox *>(_storageSumResult);
        auto& argA = a; auto& argB = b;
    {
        auto result = *(argA.mData) + *(argB.mData);
        // RVO: constructor runs directly in storage
        new (_returnSlot) IntBox{result};
    }}
    auto& sumResult = *std::launder(reinterpret_cast<IntBox *>(_storageSumResult));
    // END EXPRESSION

    /*
        auto productResult = product(a, b); 
    */
    // BEGIN EXPRESSION
    alignas(IntBox) uint8_t _storageProductResult[sizeof(IntBox)];
    {   auto* _returnSlot = reinterpret_cast<IntBox *>(_storageProductResult);
        auto& argA = a; auto& argB = b; 
    {
        auto result = *(argA.mData) * *(argB.mData);
        auto retval = IntBox{result};
        // No RVO: copy constructed into storage
        new (_returnSlot) IntBox{retval};
    }}
    auto& productResult = *std::launder(reinterpret_cast<IntBox *>(_storageProductResult));
    // END EXPRESSION

    /*
        std::cout << product(a, b).get() << std::endl;
    */
    // BEGIN EXPRESSION
    alignas(IntBox) uint8_t _tmp1[sizeof(IntBox)];
    auto _tmpIntBox1 = reinterpret_cast<IntBox *>(_tmp1);
    {   auto* _returnSlot = _tmpIntBox1; 
        auto& argA = a; auto& argB = b; 
    {
        auto result = *(argA.mData) * *(argB.mData);
        auto retval = IntBox{result};
        new (_returnSlot) IntBox(std::move(retval));
    }}
    std::cout << _tmpIntBox1->get() << std::endl;
    _tmpIntBox1->~IntBox();
    // END EXPRESSION

    // compiler normally would automatically generate these destructor calls
    // however in this expanded version, sumResult and produceResult were made by new expression
    // thus we manually call the dtors, to match the behavior of demo()
    productResult.~IntBox();
    sumResult.~IntBox();
    return 0;
}

int main() {
    std::cout << "DEMO:" << std::endl;
    demo();

    std::cout << "DEMO EXPANDED:" << std::endl;
    demoExpanded();
}