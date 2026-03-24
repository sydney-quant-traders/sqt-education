// 02_maybe_with_templates.cpp
//
// REFACTORED: A single maybe<T> class template replaces all the
// duplicated maybe_int, maybe_string, maybe_double structs.
//
// Paste this into cppinsights.io to see the compiler generate
// separate structs for each T — exactly what we wrote by hand before.
//
// Compile: g++ -std=c++17 -o 02 02_maybe_with_templates.cpp

#include <iostream>
#include <stdexcept>
#include <string>

template <typename T>
struct maybe {
    bool has_value;
    T value;

    maybe() : has_value(false), value{} {}
    maybe(T v) : has_value(true), value(v) {}

    bool has() const { return has_value; }

    const T& get() const {
        if (!has_value) throw std::runtime_error("maybe is empty");
        return value;
    }

    // Adding a new method once applies to ALL instantiations
    T value_or(const T& fallback) const {
        return has_value ? value : fallback;
    }
};

// --- Usage: same as before, but one template serves all types ---

maybe<int> find_age(const std::string& name) {
    if (name == "Alice") return maybe<int>(30);
    if (name == "Bob")   return maybe<int>(25);
    return maybe<int>();
}

maybe<std::string> find_email(const std::string& name) {
    if (name == "Alice") return maybe<std::string>("alice@example.com");
    return maybe<std::string>();
}

maybe<double> find_balance(const std::string& name) {
    if (name == "Alice") return maybe<double>(1234.56);
    return maybe<double>();
}

int main() {
    auto age   = find_age("Alice");
    auto email = find_email("Alice");
    auto bal   = find_balance("Bob");

    if (age.has())   std::cout << "Age:     " << age.get()   << "\n";
    if (email.has()) std::cout << "Email:   " << email.get() << "\n";

    // value_or — works for every T, written once
    std::cout << "Balance: " << bal.value_or(0.0) << "\n";

    // The compiler generates maybe<int>, maybe<std::string>, maybe<double>
    // but NOT maybe<char> — because we never used it.
    // Verify this in cppinsights.io!
}
