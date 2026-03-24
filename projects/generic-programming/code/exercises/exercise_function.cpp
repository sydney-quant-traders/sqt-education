// exercise_function.cpp — After Part 2 (Type Erasure + Variadic Templates)
//
// EXERCISE: Implement a simplified version of std::function.
//
// Function<R(Args...)> is a type-erased callable wrapper. It can hold:
//   - Function pointers
//   - Lambdas
//   - Any object with operator()
//
// It uses the concept/model type erasure pattern you just learned,
// combined with variadic templates.
//
// Simplifications (vs. the real std::function):
//   - Take callables by value (no forwarding references / move semantics)
//   - No small buffer optimization
//   - No allocator support
//
// YOUR TASK: Fill in every spot marked TODO. Once you're done,
// the code should compile and print "All tests passed!".
//
// Compile: g++ -std=c++17 -o exercise_function exercise_function.cpp
//
// Hints:
//   - The primary template is declared but never defined.
//     The partial specialization for R(Args...) does all the work.
//   - concept_ is an abstract base with a virtual operator().
//   - model_<F> stores a concrete callable F and delegates operator() to it.
//   - The Function constructor is a template that accepts any callable F.

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// Primary template — intentionally left undefined.
// Only the partial specialization below is used.
template <typename Signature>
class Function;

// Partial specialization: matches Function<R(Args...)>
template <typename R, typename... Args>
class Function<R(Args...)> {

    // ----------------------------------------------------------
    // TODO: Define concept_ — an abstract base class with:
    //   - A virtual destructor
    //   - A pure virtual method: R call(Args... args)
    // ----------------------------------------------------------


    // ----------------------------------------------------------
    // TODO: Define model_<F> — a class template that:
    //   - Inherits from concept_
    //   - Stores a value of type F (the concrete callable)
    //   - Constructs from an F value
    //   - Implements call() by invoking the stored callable
    //     with the forwarded arguments: return callable_(args...)
    // ----------------------------------------------------------


    std::unique_ptr<concept_> impl_;

public:
    Function() = default;

    // ----------------------------------------------------------
    // TODO: Templated constructor that accepts any callable F.
    //   - Create a model_<F> on the heap wrapping the callable
    //   - Store it in impl_
    // ----------------------------------------------------------


    R operator()(Args... args) const {
        if (!impl_) throw std::runtime_error("calling empty Function");

        // ----------------------------------------------------------
        // TODO: Delegate to impl_->call(...), passing along the args.
        //   Return the result.
        // ----------------------------------------------------------

    }

    explicit operator bool() const {
        return impl_ != nullptr;
    }
};

// ============================================================
// Tests — do not modify below this line
// ============================================================

int add(int a, int b) { return a + b; }
double multiply(double a, double b) { return a * b; }
void greet(const std::string& name) { std::cout << "  Hello, " << name << "!\n"; }

struct Multiplier {
    int factor;
    int operator()(int x) const { return x * factor; }
};

int main() {
    int passed = 0;

    // Test 1: function pointer
    {
        Function<int(int, int)> f = add;
        if (f(3, 4) != 7) { std::cout << "FAIL: add(3,4) should be 7\n"; return 1; }
        passed++;
    }

    // Test 2: different function pointer
    {
        Function<double(double, double)> f = multiply;
        if (f(2.5, 4.0) != 10.0) { std::cout << "FAIL: multiply(2.5,4) should be 10\n"; return 1; }
        passed++;
    }

    // Test 3: lambda (no capture)
    {
        Function<int(int, int)> f = [](int a, int b) { return a - b; };
        if (f(10, 3) != 7) { std::cout << "FAIL: lambda(10,3) should be 7\n"; return 1; }
        passed++;
    }

    // Test 4: lambda with capture
    {
        int offset = 100;
        Function<int(int)> f = [offset](int x) { return x + offset; };
        if (f(5) != 105) { std::cout << "FAIL: captured lambda(5) should be 105\n"; return 1; }
        passed++;
    }

    // Test 5: callable object
    {
        Function<int(int)> f = Multiplier{3};
        if (f(7) != 21) { std::cout << "FAIL: Multiplier{3}(7) should be 21\n"; return 1; }
        passed++;
    }

    // Test 6: void return type
    {
        Function<void(const std::string&)> f = greet;
        std::cout << "  (Test 6 output) ";
        f("World"); // should print "Hello, World!"
        passed++;
    }

    // Test 7: bool conversion
    {
        Function<int(int)> empty;
        Function<int(int)> full = [](int x) { return x; };
        if (empty) { std::cout << "FAIL: empty function should be falsy\n"; return 1; }
        if (!full) { std::cout << "FAIL: assigned function should be truthy\n"; return 1; }
        passed++;
    }

    // Test 8: exception on empty call
    {
        Function<int()> f;
        bool caught = false;
        try { f(); } catch (const std::runtime_error&) { caught = true; }
        if (!caught) { std::cout << "FAIL: calling empty function should throw\n"; return 1; }
        passed++;
    }

    // Test 9: reassignment
    {
        Function<int(int)> f = [](int x) { return x * 2; };
        if (f(5) != 10) { std::cout << "FAIL: first lambda(5) should be 10\n"; return 1; }
        f = [](int x) { return x * 3; };
        if (f(5) != 15) { std::cout << "FAIL: reassigned lambda(5) should be 15\n"; return 1; }
        passed++;
    }

    std::cout << "All tests passed! (" << passed << "/9)\n";
}
