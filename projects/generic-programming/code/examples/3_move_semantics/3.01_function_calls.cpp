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

    // returns a reference to the data contained
    int& get() {
        return *data_;
    }

    int *data_;
};

// --- Library Functions ---

intbox get_three() {
    // construct `a` in this stack frame
    intbox a{3};
    // move construct `a` into the return slot
    return a;
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
    intbox a{5};
    intbox b{6};

    // creates a temporary that is destroyed when expression ends
    std::cout << product(a, b).get() << std::endl;

    // equivalent to intbox product_result{product(a, get_three())};
    auto product_result = product(a, b);

    // Do whatever we want with product_result
    std::cout << "Modify product result" << std::endl;
    product_result.get() = 42;
    std::cout << product_result.get() << std::endl;

    return 0;
}

int demo_expanded() {
    intbox a{5};
    intbox b{6};

    /*
        std::cout << product(a, b).get();
    */
    // BEGIN EXPRESSION
    alignas(intbox) uint8_t _tmp1[sizeof(intbox)];
    auto _tmp_intbox1 = reinterpret_cast<intbox *>(_tmp1);
    {   intbox *_return_slot = _tmp_intbox1; 
        intbox& arg_a = a; intbox& arg_b = b; 
    {
        auto result = *(arg_a.data_) * *(arg_b.data_);
        intbox retval{result};

        // use retval to construct into return slot
        new (_return_slot) intbox(std::move(retval));
    }}
    std::cout << _tmp_intbox1->get() << std::endl;
    _tmp_intbox1->~intbox();
    // END EXPRESSION


    /*
        auto product_result = product(a, b); 
    */
    // BEGIN EXPRESSION
    alignas(intbox) uint8_t _tmp2[sizeof(intbox)];
    auto _tmp_intbox2 = reinterpret_cast<intbox *>(_tmp2);
    {   intbox *_return_slot = _tmp_intbox2;
        intbox& arg_a = a; intbox& arg_b = b; 
    {
        auto result = *(arg_a.data_) * *(arg_b.data_);
        intbox retval{result};

        // use retval to construct into return slot
        new (_return_slot) intbox(std::move(retval));
    }}
    intbox product_result{std::move(*_tmp_intbox2)}; // slightly inaccurate
    _tmp_intbox2->~intbox();
    // END EXPRESSION

    // Do whatever we want with product_result
    std::cout << "Modify product result" << std::endl;
    product_result.get() = 42;
    std::cout << product_result.get() << std::endl;

    return 0;
}

int main() {
    std::cout << "DEMO:" << std::endl;
    demo();

    std::cout << "DEMO EXPANDED:" << std::endl;
    demo_expanded();
}