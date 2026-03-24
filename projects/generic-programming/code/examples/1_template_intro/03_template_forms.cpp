// 03_template_forms.cpp
//
// The three forms of templates: function, class, and variable templates.
//
// Compile: g++ -std=c++17 -o 03 03_template_forms.cpp

#include <iostream>
#include <string>
#include <type_traits>

// ============================================================
// 1. FUNCTION TEMPLATES
// ============================================================

// The compiler generates a concrete function for each T used.
template <typename T>
T max_of(T a, T b) {
    return (a > b) ? a : b;
}

// Function templates can have multiple type parameters.
template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// ============================================================
// 2. CLASS TEMPLATES
// ============================================================

// A simple generic pair — the compiler generates a struct for
// each combination of T, U actually used.
template <typename T, typename U>
struct pair {
    T first;
    U second;

    void print() const {
        std::cout << "(" << first << ", " << second << ")\n";
    }
};

// Non-type template parameters: the value is part of the type.
template <typename T, int N>
struct fixed_array {
    T data[N];

    int size() const { return N; }

    T& operator[](int i) { return data[i]; }
    const T& operator[](int i) const { return data[i]; }
};

// ============================================================

int main() {
    // --- Function templates ---
    std::cout << "max_of(3, 7)       = " << max_of(3, 7)             << "\n";
    std::cout << "max_of(3.1, 2.9)   = " << max_of(3.1, 2.9)        << "\n";
    std::cout << "add(1, 2.5)        = " << add(1, 2.5)              << "\n";

    // --- Class templates ---
    pair<std::string, int> p{"Alice", 30};
    p.print();

    fixed_array<double, 3> arr{};
    arr[0] = 1.1; arr[1] = 2.2; arr[2] = 3.3;
    std::cout << "arr size = " << arr.size() << ", arr[1] = " << arr[1] << "\n";
}
