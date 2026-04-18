// clang++ -std=c++20 -fno-elide-constructors 3.00_binding.cpp

#include <utility>
#include <cstdint>
#include <new>
#include <iostream>

// --- Utility: stores a callable to run when the scope ends ---

template <typename F>
struct scope_exit {
    F f_;
    scope_exit(F f) : f_(f) {}
    ~scope_exit() { f_(); }

    scope_exit(const scope_exit&) = delete;
    scope_exit& operator=(const scope_exit&) = delete;
};

// --- Library Types ---

struct intbox {
    intbox(int x) : data_{new int(x)} {
        std::cout << "constructor" << std::endl;
    }
    ~intbox() { 
        delete data_; 
        std::cout << "destructor" << std::endl;
    }

    intbox(const intbox& other) : data_{new int(*(other.data_))} {
        std::cout << "copy constructor" << std::endl;
    };
    intbox& operator=(const intbox& other) = delete;
    intbox(intbox&& other) : data_{std::exchange(other.data_, nullptr)} {
        std::cout << "move constructor" << std::endl;
    }
    intbox& operator=(intbox&& other) = delete;

    // returns a reference to the data contained
    int& get() {
        return *data_;
    }

    int *data_;
};

// --- Library Functions ---

// RVO always occurs here
intbox sum(intbox& a, intbox& b) {
    auto result = *(a.data_) + *(b.data_);
    return intbox(result);
}

// RVO does not occur
// Prevent NVRO with -fno-elide-constructors flag
intbox product(intbox& a, intbox& b) {
    auto result = *(a.data_) * *(b.data_);
    intbox retval{result};
    return retval;
}

// --- Application Code ---

int demo() {
    intbox a(5);
    intbox b(6);

    // RVO: return slot is to sum_result storage
    auto sum_result = sum(a, b);

    // No RVO: return slot is STILL to product_result storage
    // - however an extra move occurs in the function body
    auto product_result = product(a, b);

    // No RVO: return slot is to a temporary, rather than an intbox object
    std::cout << product(a, b).get() << std::endl;

    return 0;
}

int demo_expanded() {
    intbox a(5);
    intbox b(6);

    /*
        auto sum_result = sum(a, static_cast<intbox&&>(b));
    */
    alignas(intbox) uint8_t _storage_sum_result[sizeof(intbox)];
    {   intbox *_return_slot = reinterpret_cast<intbox *>(_storage_sum_result);
        intbox& arg_a = a; intbox& arg_b = b;
    {
        auto result = *(arg_a.data_) + *(arg_b.data_);
        // RVO: constructor runs directly in storage
        new (_return_slot) intbox(result);
    }}
    intbox& sum_result = *std::launder(reinterpret_cast<intbox *>(_storage_sum_result));
    scope_exit _sum_result_guard([&]{sum_result.~intbox();});

    /*
        auto product_result = product(a, b); 
    */
    // BEGIN EXPRESSION
    alignas(intbox) uint8_t _storage_product_result[sizeof(intbox)];
    {   intbox *_return_slot = reinterpret_cast<intbox *>(_storage_product_result);
        intbox& arg_a = a; intbox& arg_b = b; 
    {
        auto result = *(arg_a.data_) * *(arg_b.data_);
        intbox retval{result};
        // No RVO: move constructed into storage
        new (_return_slot) intbox(std::move(retval));
    }}
    intbox& product_result = *std::launder(reinterpret_cast<intbox *>(_storage_product_result));
    scope_exit _product_result_guard([&]{product_result.~intbox();});
    // END EXPRESSION

    /*
        std::cout << product(a, b).get() << std::endl;
    */
    // BEGIN EXPRESSION
    alignas(intbox) uint8_t _tmp1[sizeof(intbox)];
    auto _tmp_intbox1 = reinterpret_cast<intbox *>(_tmp1);
    {   intbox *_return_slot = _tmp_intbox1; 
        intbox& arg_a = a; intbox& arg_b = b; 
    {
        auto result = *(arg_a.data_) * *(arg_b.data_);
        intbox retval{result};
        new (_return_slot) intbox(std::move(retval));
    }}
    std::cout << _tmp_intbox1->get() << std::endl;
    _tmp_intbox1->~intbox();
    // END EXPRESSION

    return 0;
}

int main() {
    std::cout << "DEMO:" << std::endl;
    demo();

    std::cout << "DEMO EXPANDED:" << std::endl;
    demo_expanded();
}