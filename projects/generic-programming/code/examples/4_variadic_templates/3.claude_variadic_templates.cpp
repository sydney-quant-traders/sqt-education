// 08_variadic_templates.cpp
//
// DETOUR: Variadic templates — needed for the type-erased
// std::function exercise in Part 2.4.
//
// Key concepts:
//   - Parameter packs: template<typename... Args>
//   - Pack expansion: f(args...)
//   - Recursive unpacking
//   - Fold expressions (C++17)
//
// Compile: g++ -std=c++17 -o 08 08_variadic_templates.cpp

#include <iostream>
#include <memory>
#include <string>

// ============================================================
// 1. BASIC VARIADIC FUNCTION
// ============================================================

// Base case: no arguments
void print() {
    std::cout << "\n";
}

// Recursive case: peel off the first argument, recurse on the rest
template <typename T, typename... Rest>
void print(T first, Rest... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) std::cout << ", ";
    print(rest...);  // pack expansion: rest... expands to each remaining arg
}

// ============================================================
// 2. FOLD EXPRESSIONS (C++17) — much cleaner
// ============================================================

template <typename... Args>
auto sum(Args... args) {
    return (args + ...);  // unary right fold: a1 + (a2 + (a3 + ...))
}

template <typename... Args>
void print_all(Args... args) {
    // Binary left fold with comma operator
    ((std::cout << args << " "), ...) ;
    std::cout << "\n";
}

// ============================================================
// 3. sizeof... — count the pack
// ============================================================

template <typename... Args>
constexpr int count() {
    return sizeof...(Args);
}

// ============================================================
// 4. PERFECT FORWARDING (simplified — no move semantics)
//
// This is how std::make_unique works: it takes any arguments
// and forwards them to T's constructor.
// ============================================================

template <typename T, typename... Args>
std::unique_ptr<T> my_make_unique(Args... args) {
    return std::unique_ptr<T>(new T(args...));  // pack expansion in constructor call
}

struct point {
    double x, y;
    point(double x, double y) : x(x), y(y) {}
    void print() const { std::cout << "(" << x << ", " << y << ")\n"; }
};

// ============================================================
// 5. VARIADIC CLASS TEMPLATE — tuple sketch
// ============================================================

// Empty base case
template <typename... Ts>
struct my_tuple {};

// Recursive case: store the first element, inherit the rest
template <typename T, typename... Rest>
struct my_tuple<T, Rest...> : my_tuple<Rest...> {
    T value;
    my_tuple(T v, Rest... rest) : my_tuple<Rest...>(rest...), value(v) {}
};

// ============================================================

int main() {
    // 1. Recursive variadic print
    std::cout << "print: ";
    print(1, "hello", 3.14, 'x');

    // 2. Fold expressions
    std::cout << "sum:   " << sum(1, 2, 3, 4, 5) << "\n";
    std::cout << "all:   ";
    print_all(42, "world", 2.718);

    // 3. sizeof...
    std::cout << "count: " << count<int, double, std::string>() << "\n";

    // 4. make_unique with forwarded args
    auto p = my_make_unique<point>(3.0, 4.0);
    std::cout << "point: ";
    p->print();

    // 5. Variadic tuple
    my_tuple<int, double, std::string> t(1, 2.5, "three");
    // (accessing elements would need get<N> — omitted for brevity)
    std::cout << "tuple created successfully\n";
}
