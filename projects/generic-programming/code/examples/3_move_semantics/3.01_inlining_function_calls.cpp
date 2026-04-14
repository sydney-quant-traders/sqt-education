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

    // these functions obey move and ownership semantics
    intbox(const intbox& other) : data_{new int(*(other.data_))} {
        std::cout << "copy constructor" << std::endl;
    };
    intbox& operator=(const intbox& other) = delete;
    intbox(intbox&& other) : data_{std::exchange(other.data_, nullptr)} {
        std::cout << "move constructor" << std::endl;
    }
    intbox& operator=(intbox&& other) = delete;

    int *data_;
};

// --- Library Functions ---

// RVO always occurs here
intbox sum(intbox& a, intbox&& b) {
    auto result = *(a.data_) + *(b.data_);
    return intbox(result);
}

// RVO does not occur
// Prevent NVRO with -fno-elide-constructors flag
intbox product(intbox& a, intbox&& b) {
    auto result = *(a.data_) * *(b.data_);
    intbox retval{result};
    return retval;
}

// --- Application Code ---

int demo() {
    intbox a(5);
    intbox b(6);

    // call sum
    auto sum_result = sum(a, static_cast<intbox&&>(b));
    auto product_result = product(a, static_cast<intbox&&>(b));

    return 0;
}

int demo_expanded() {
    intbox a(5);
    intbox b(6);

    /*
        auto sum_result = sum(a, static_cast<intbox&&>(b));
    */
    alignas(intbox) uint8_t _sum_result_temporary[sizeof(intbox)];
    { intbox& arg_a = a; intbox&& arg_b = static_cast<intbox&&>(b); {
        auto result = *(arg_a.data_) + *(arg_b.data_);
        // RVO: returned expr is a constructor call: runs directly in storage
        new (reinterpret_cast<intbox *>(_sum_result_temporary)) intbox(result);
    }}
    intbox& sum_result = *std::launder(reinterpret_cast<intbox*>(_sum_result_temporary));
    scope_exit _sum_result_guard([&]{sum_result.~intbox();});

    /*
        auto product_result = product(a, static_cast<intbox&&>(b)); 
    */
    alignas(intbox) uint8_t _product_result_temporary[sizeof(intbox)];
    { intbox& arg_a = a; intbox&& arg_b = static_cast<intbox&&>(b); {
        auto result = *(arg_a.data_) * *(arg_b.data_);
        intbox retval{result};
        // No RVO: returned expr is a named intbox: used as argument to a constructor call
        // whether the returned expr is std::moved or not depends on presence of move constructors?
        new (reinterpret_cast<intbox *>(_product_result_temporary)) intbox(retval);
    }}
    intbox& product_result = *std::launder(reinterpret_cast<intbox*>(_product_result_temporary));
    scope_exit product_result_guard([&]{product_result.~intbox();});

    return 0;
}

int main() {
    demo();
    demo_expanded();
}