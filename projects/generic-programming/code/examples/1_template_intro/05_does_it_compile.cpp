// 05_does_it_compile.cpp
//
// "DOES IT COMPILE?" exercises.
//
// Each snippet is wrapped in a namespace. Predict whether it compiles
// or fails, then uncomment the call in main() to verify.
//
// Key insight: templates are only instantiated when USED. Code that
// would be invalid might compile if no one ever calls it with the
// problematic type. This is the foundation of SFINAE.
//
// Compile: g++ -std=c++17 -o 05 05_does_it_compile.cpp

#include <iostream>
#include <string>

// ============================================================
// SNIPPET 1: Calling .size() on T
// ============================================================
namespace snippet1 {
    template <typename T>
    int get_size(T x) {
        return x.size();
    }

    void test() {
        std::string s = "hello";
        std::cout << "1: " << get_size(s) << "\n";  // string has .size()

        // Does this compile?
        // int n = 42;
        // std::cout << get_size(n) << "\n";   // int has no .size()
    }
}

// ============================================================
// SNIPPET 2: Unused member function
// ============================================================
namespace snippet2 {
    template <typename T>
    struct wrapper {
        T value;

        void print() {
            std::cout << value << "\n";
        }

        // This method calls .clear() on T. Does the struct
        // fail to compile even if we never call reset()?
        void reset() {
            value.clear();
        }
    };

    struct no_clear {
        int x;
        friend std::ostream& operator<<(std::ostream& os, const no_clear& nc) {
            return os << nc.x;
        }
    };

    void test() {
        wrapper<no_clear> w{no_clear{42}};
        w.print();   // Only calls print(), never reset()
        // Does this compile? Think about when member functions
        // of class templates are instantiated...
    }
}

// ============================================================
// SNIPPET 3: Ambiguous deduction
// ============================================================
namespace snippet3 {
    template <typename T>
    T add(T a, T b) {
        return a + b;
    }

    void test() {
        std::cout << "3: " << add(1, 2) << "\n";     // Both int — OK

        // Does this compile? Both args must deduce the SAME T.
        // std::cout << add(1, 2.5) << "\n";   // int vs double
    }
}

// ============================================================
// SNIPPET 4: Non-type template parameter mismatch
// ============================================================
namespace snippet4 {
    template <int N>
    struct fixed_buffer {
        char data[N];
        int size() const { return N; }
    };

    void test() {
        fixed_buffer<64> a;
        fixed_buffer<128> b;
        std::cout << "4: a.size()=" << a.size() << " b.size()=" << b.size() << "\n";

        // Does this compile? Are a and b the same type?
        // a = b;
    }
}

// ============================================================
// SNIPPET 5: Dependent name lookup
// ============================================================
namespace snippet5 {
    template <typename T>
    struct container {
        using value_type = T;
    };

    template <typename C>
    void print_value_type() {
        // Does this compile without 'typename'?
        // The compiler doesn't know C::value_type is a type
        // until C is substituted — it might be a static member.
        typename C::value_type val{};
        std::cout << "5: default value = " << val << "\n";
    }

    void test() {
        print_value_type<container<int>>();
    }
}

// ============================================================
// SNIPPET 6: Static assert inside a template
// ============================================================
namespace snippet6 {
    template <typename T>
    T safe_divide(T a, T b) {
        static_assert(std::is_arithmetic_v<T>, "safe_divide requires an arithmetic type");
        if (b == T{}) throw std::runtime_error("division by zero");
        return a / b;
    }

    void test() {
        std::cout << "6: " << safe_divide(10, 3) << "\n";
        std::cout << "6: " << safe_divide(10.0, 3.0) << "\n";

        // Does this compile?
        // std::cout << safe_divide(std::string("a"), std::string("b")) << "\n";
    }
}

// ============================================================

int main() {
    snippet1::test();
    snippet2::test();
    snippet3::test();
    snippet4::test();
    snippet5::test();
    snippet6::test();
}
