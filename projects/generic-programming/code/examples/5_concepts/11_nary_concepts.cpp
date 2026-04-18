// 11_nary_concepts.cpp
//
// N-ARY CONCEPTS — concepts that constrain relationships
// between multiple types.
//
// Concepts are compile-time boolean functions. They can be unary
// (one type), binary (two types), or n-ary. This lets you express
// relationships like "T is convertible to U" or "T and U can be added."
//
// Compile: clang++ -std=c++20 -o 11 11_nary_concepts.cpp
//     or:  g++ -std=c++20 -o 11 11_nary_concepts.cpp  (GCC 10+)

#include <iostream>
#include <string>
#include <type_traits>
#include <concepts>

// ============================================================
// 1. BINARY CONCEPTS — relationships between two types
// ============================================================

// "T can be explicitly converted to U"
template <typename From, typename To>
concept ExplicitlyConvertible = requires(From f) {
    static_cast<To>(f);
};

// "T and U can be added, producing some result"
template <typename T, typename U>
concept Addable = requires(T a, U b) {
    { a + b };
};

// "T and U can be added, and the result is convertible to R"
template <typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
concept AddableTo = Addable<T, U> && std::convertible_to<R, T>;

// ============================================================
// 2. USING BINARY CONCEPTS
// ============================================================

// Only allow addition when the types are compatible
template <typename T, typename U>
    requires Addable<T, U>
auto flexible_add(T a, U b) {
    return a + b;
}

// Require that From is convertible to To
template <typename From, typename To>
    requires ExplicitlyConvertible<From, To>
To convert(From value) {
    return static_cast<To>(value);
}

// ============================================================
// 3. STANDARD LIBRARY BINARY CONCEPTS
// ============================================================

// std::same_as<T, U>         — T and U are the same type
// std::convertible_to<T, U>  — T is implicitly convertible to U
// std::derived_from<T, U>    — T inherits from U
// std::assignable_from<T, U> — T = U is valid
// std::common_with<T, U>     — T and U share a common type

// Constrain to types that share a common type for comparison
template <typename T, typename U>
    requires std::totally_ordered<T> && std::totally_ordered<U>
          && std::common_with<T, U>
auto generic_max(T a, U b) {
    using Common = std::common_type_t<T, U>;
    return (Common(a) > Common(b)) ? Common(a) : Common(b);
}

// ============================================================
// 4. COMPOSING CONCEPTS — building complex constraints
// ============================================================

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <typename T>
concept Ordered = std::totally_ordered<T>;

// A type that can be used in statistical computations
template <typename T, typename U>
concept StatCompatible = Numeric<T> && Numeric<U>
    && Addable<T, U>
    && std::common_with<T, U>;

template <typename T, typename U>
    requires StatCompatible<T, U>
auto weighted_average(T value, U weight) {
    using R = std::common_type_t<T, U>;
    return R(value) * R(weight);
}

// ============================================================
// 5. N-ARY — more than two types
// ============================================================

// All types must be convertible to a common target type
template <typename Target, typename... Sources>
concept AllConvertibleTo = (std::convertible_to<Sources, Target> && ...);

template <typename Target, typename... Args>
    requires AllConvertibleTo<Target, Args...>
Target sum_as(Args... args) {
    return (Target(args) + ...);
}

// ============================================================

int main() {
    // Binary concepts
    std::cout << "flexible_add(1, 2.5) = " << flexible_add(1, 2.5) << "\n";
    std::cout << "flexible_add(str, str) = "
              << flexible_add(std::string("he"), std::string("llo")) << "\n";

    std::cout << "convert<int,double>(42) = " << convert<int, double>(42) << "\n";

    // Standard library binary concepts
    std::cout << "generic_max(3, 4.5) = " << generic_max(3, 4.5) << "\n";

    // Composed binary concept
    std::cout << "weighted_average(10, 0.5) = " << weighted_average(10, 0.5) << "\n";

    // N-ary concept
    std::cout << "sum_as<double>(1, 2, 3.5) = " << sum_as<double>(1, 2, 3.5) << "\n";
    std::cout << "sum_as<int>(1, 2, 3) = " << sum_as<int>(1, 2, 3) << "\n";
}
