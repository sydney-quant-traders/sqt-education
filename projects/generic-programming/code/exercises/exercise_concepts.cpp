// exercise_concepts.cpp — After Part 3 (Concepts)
//
// EXERCISE: Add concepts to unconstrained templates.
//
// Below are several template functions that work for some types and
// fail with cryptic errors for others. Your job: write concepts that
// express the requirements, then apply them as constraints.
//
// After you're done, the GOOD calls should compile, and the BAD calls
// (commented out) should fail with a CLEAR concept error rather than
// a wall of template noise. Uncomment each BAD line to verify you get
// a clean error message.
//
// YOUR TASK: Fill in every spot marked TODO.
//
// Compile: clang++ -std=c++20 -o exercise_concepts exercise_concepts.cpp
//     or:  g++ -std=c++20 -o exercise_concepts exercise_concepts.cpp  (GCC 10+)

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================
// PART A: Constrain numeric_average
// ============================================================

// TODO: Write a concept called "Numeric" that requires:
//   - T can be constructed from the integer literal 0
//   - Two values of T can be added with +
//   - A value of T can be divided by an int
//
// Hint: use a requires expression with these checks:
//   T{0}
//   { a + b } -> std::convertible_to<T>
//   { a / int{} } -> std::convertible_to<T>


// TODO: Constrain this function with your Numeric concept.
template <typename T>
T numeric_average(T a, T b) {
    return (a + b) / 2;
}


// ============================================================
// PART B: Constrain print_all
// ============================================================

// TODO: Write a concept called "Printable" that requires:
//   - A value of T can be streamed to std::ostream with <<
//
// Hint:
//   { os << val } -> std::convertible_to<std::ostream&>


// TODO: Constrain this function with your Printable concept.
template <typename... Args>
void print_all(Args... args) {
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}


// ============================================================
// PART C: Constrain get_size
// ============================================================

// TODO: Write a concept called "Sizable" that requires:
//   - T has a .size() method
//   - The return type of .size() is convertible to std::size_t


// TODO: Constrain this function with your Sizable concept.
template <typename T>
std::size_t get_size(const T& container) {
    return container.size();
}


// ============================================================
// PART D: Constrain merge (binary concept)
// ============================================================

// TODO: Write a concept called "Appendable" that requires:
//   - For types Container and Element:
//   - Container has a .push_back(Element) method
//   - Container has .begin() and .end() methods
//
// This is a BINARY concept: it relates two types.
//
// Hint: template <typename Container, typename Element>


// TODO: Constrain this function with your Appendable concept.
template <typename Container, typename Element>
void append_all(Container& dest, const Element* arr, std::size_t n) {
    for (std::size_t i = 0; i < n; i++) {
        dest.push_back(arr[i]);
    }
}


// ============================================================
// Tests — do not modify below this line
// ============================================================

struct NotPrintable {};

int main() {
    int passed = 0;

    // --- Part A: numeric_average ---
    // GOOD: works with int and double
    if (numeric_average(10, 20) != 15) { std::cout << "FAIL A1\n"; return 1; }
    if (numeric_average(3.0, 5.0) != 4.0) { std::cout << "FAIL A2\n"; return 1; }
    passed++;

    // BAD: should fail with a concept error (uncomment to test):
    // numeric_average(std::string("a"), std::string("b"));

    // --- Part B: print_all ---
    // GOOD: works with printable types
    std::cout << "  (Part B) ";
    print_all(1, 2.5, "hello");
    passed++;

    // BAD: should fail with a concept error (uncomment to test):
    // print_all(NotPrintable{});

    // --- Part C: get_size ---
    // GOOD: works with containers
    std::string s = "hello";
    std::vector<int> v = {1, 2, 3};
    if (get_size(s) != 5) { std::cout << "FAIL C1\n"; return 1; }
    if (get_size(v) != 3) { std::cout << "FAIL C2\n"; return 1; }
    passed++;

    // BAD: should fail with a concept error (uncomment to test):
    // get_size(42);

    // --- Part D: append_all ---
    // GOOD: works with vector and compatible element type
    std::vector<int> dest;
    int arr[] = {10, 20, 30};
    append_all(dest, arr, 3);
    if (dest.size() != 3 || dest[0] != 10 || dest[2] != 30) {
        std::cout << "FAIL D1\n"; return 1;
    }
    passed++;

    // BAD: should fail with a concept error (uncomment to test):
    // std::string str_dest;
    // int nums[] = {1, 2, 3};
    // append_all(str_dest, nums, 3);  // string has push_back(char), not push_back(int)... or does it?

    std::cout << "All tests passed! (" << passed << "/4)\n";
}
