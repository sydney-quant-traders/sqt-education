// 04_template_deduction.cpp
//
// Template argument deduction: how the compiler figures out T
// from the arguments you pass — without you writing <T> explicitly.
//
// Compile: g++ -std=c++17 -o 04 04_template_deduction.cpp

#include <iostream>
#include <string>

template <typename T>
void show_type(T value) {
    // __PRETTY_FUNCTION__ is a GCC/Clang extension that includes
    // the deduced template arguments in the function signature.
    std::cout << __PRETTY_FUNCTION__ << "\n";
    std::cout << "  value = " << value << "\n\n";
}

template <typename T, typename U>
auto multiply(T a, U b) {
    std::cout << __PRETTY_FUNCTION__ << "\n";
    return a * b;
}

// When the compiler can't deduce T (it's only in the return type),
// you MUST specify it explicitly.
template <typename T>
T zero() {
    return T{};
}

// Deduction with references: T is deduced differently depending on
// whether the parameter is by-value, by-ref, or by-const-ref.
template <typename T>
void by_value(T x)        { std::cout << "by_value:     " << __PRETTY_FUNCTION__ << "\n"; }

template <typename T>
void by_ref(T& x)         { std::cout << "by_ref:       " << __PRETTY_FUNCTION__ << "\n"; }

template <typename T>
void by_const_ref(const T& x) { std::cout << "by_const_ref: " << __PRETTY_FUNCTION__ << "\n"; }

int main() {
    // --- Basic deduction ---
    show_type(42);               // T = int
    show_type(3.14);             // T = double
    show_type("hello");          // T = const char*
    show_type(std::string("hello")); // T = std::string

    // --- Multi-parameter deduction ---
    auto result = multiply(3, 2.5);  // T = int, U = double
    std::cout << "  result = " << result << "\n\n";

    // --- Explicit specification (deduction impossible) ---
    std::cout << "zero<int>()    = " << zero<int>()    << "\n";
    std::cout << "zero<double>() = " << zero<double>() << "\n";
    std::cout << "zero<std::string>() = \"" << zero<std::string>() << "\"\n\n";

    // --- How references affect deduction ---
    int x = 42;
    const int cx = 42;

    by_value(x);        // T = int       (const/ref stripped)
    by_value(cx);       // T = int       (const stripped)
    by_ref(x);          // T = int
    by_ref(cx);         // T = const int (const preserved!)
    by_const_ref(x);    // T = int       (const added by parameter)
    by_const_ref(cx);   // T = int       (const already in parameter)
}
