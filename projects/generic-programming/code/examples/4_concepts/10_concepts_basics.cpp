// 10_concepts_basics.cpp
//
// C++20 CONCEPTS — syntax and usage.
//
// Before C++20, template constraints were enforced by documentation
// and horrific error messages. Concepts make constraints explicit,
// checked at the call site, and readable.
//
// Compile: clang++ -std=c++20 -o 10 10_concepts_basics.cpp
//     or:  g++ -std=c++20 -o 10 10_concepts_basics.cpp  (GCC 10+)

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================
// 1. REQUIRES CLAUSE — constrain a template
// ============================================================

// Only allow arithmetic types (int, double, etc.)
template <typename T>
    requires std::is_arithmetic_v<T>
T safe_divide(T a, T b) {
    if (b == T{}) throw std::runtime_error("division by zero");
    return a / b;
}

// Equivalent shorthand: trailing requires
template <typename T>
T safe_multiply(T a, T b) requires std::is_arithmetic_v<T> {
    return a * b;
}

// ============================================================
// 2. REQUIRES EXPRESSION — test if operations are valid
// ============================================================

// A requires expression evaluates to true/false at compile time.
// It checks whether the enclosed expressions are well-formed.

template <typename T>
constexpr bool has_size = requires(T t) {
    t.size();   // is t.size() a valid expression?
};

// The requires expression can also check return types:
template <typename T>
constexpr bool has_int_size = requires(T t) {
    { t.size() } -> std::convertible_to<int>;
};

// ============================================================
// 3. CONCEPT — a named, reusable constraint
// ============================================================

template <typename T>
concept Printable = requires(std::ostream& os, T val) {
    { os << val } -> std::convertible_to<std::ostream&>;
};

template <typename T>
concept Drawable = requires(const T& t) {
    t.draw();   // T must have a draw() method callable on a const ref
};

template <typename T>
concept Container = requires(T t) {
    typename T::value_type;         // must have a nested value_type
    { t.size() } -> std::convertible_to<std::size_t>;
    { t.begin() };                  // must have begin()
    { t.end() };                    // must have end()
};

// ============================================================
// 4. USING CONCEPTS — three equivalent syntaxes
// ============================================================

// Syntax 1: requires clause
template <typename T>
    requires Printable<T>
void print_v1(const T& val) {
    std::cout << val << "\n";
}

// Syntax 2: constrained template parameter
template <Printable T>
void print_v2(const T& val) {
    std::cout << val << "\n";
}

// Syntax 3: abbreviated function template (auto + concept)
void print_v3(const Printable auto& val) {
    std::cout << val << "\n";
}

// Using the Drawable concept
void render(const Drawable auto& shape) {
    shape.draw();
}

// Using the Container concept
void print_size(const Container auto& c) {
    std::cout << "Container with " << c.size() << " elements\n";
}

// ============================================================
// 5. REQUIRES REQUIRES — inline requires expression
// ============================================================

// When you need a one-off constraint without defining a concept:
template <typename T>
    requires requires(T a, T b) { a + b; }  // yes, "requires requires"
auto add(T a, T b) {
    return a + b;
}

// ============================================================
// 6. OVERLOADING WITH CONCEPTS
// ============================================================

// For subsumption to work (compiler picks the most constrained
// overload), constraints must be expressed as concepts — not raw
// bool expressions. The compiler can see that Integral refines
// Arithmetic, but it can't see that is_integral_v "implies"
// is_arithmetic_v.

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept Integral = Arithmetic<T> && std::is_integral_v<T>;

template <typename T>
std::string describe(const T&) {
    return "unknown type";
}

template <Arithmetic T>
std::string describe(const T&) {
    return "arithmetic type";
}

template <Integral T>
std::string describe(const T&) {
    return "integral type";  // more constrained than Arithmetic
}

// ============================================================

struct circle {
    double radius;
    void draw() const { std::cout << "circle r=" << radius << "\n"; }
};

int main() {
    // 1. Requires clause
    std::cout << "divide: " << safe_divide(10.0, 3.0) << "\n";
    // safe_divide(std::string("a"), std::string("b")); // error: constraint not satisfied

    // 2. Requires expression (compile-time bool)
    std::cout << "vector has size: " << has_size<std::vector<int>> << "\n";
    std::cout << "int has size:    " << has_size<int> << "\n";

    // 3-4. Concepts in action
    print_v1(42);
    print_v2(3.14);
    print_v3("hello");

    render(circle{5.0});
    print_size(std::vector<int>{1, 2, 3});

    // 5. requires requires
    std::cout << "add: " << add(1, 2) << "\n";
    std::cout << "add: " << add(std::string("he"), std::string("llo")) << "\n";

    // 6. Overload resolution with concepts
    std::cout << "describe(42):    " << describe(42) << "\n";     // integral (most specific)
    std::cout << "describe(3.14):  " << describe(3.14) << "\n";   // arithmetic
    std::cout << "describe(\"hi\"): " << describe("hi") << "\n";  // unknown
}
