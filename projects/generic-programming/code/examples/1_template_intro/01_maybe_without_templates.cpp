// 01_maybe_without_templates.cpp
//
// MOTIVATION: Why do we need templates?
//
// This file shows what happens when you need the same wrapper logic
// for multiple unrelated types. Each maybe_T struct is nearly identical
// — only the wrapped type differs. This violates DRY and becomes
// unmaintainable as the number of types grows.
//
// Compile: g++ -std=c++17 -o 01 01_maybe_without_templates.cpp

#include <iostream>
#include <stdexcept>
#include <string>

// --- maybe_int ---

struct maybe_int {
    bool has_value;
    int value;

    maybe_int() : has_value(false), value{} {}
    maybe_int(int v) : has_value(true), value(v) {}

    bool has() const { return has_value; }

    int get() const {
        if (!has_value) throw std::runtime_error("maybe_int is empty");
        return value;
    }
};

// --- maybe_string ---

struct maybe_string {
    bool has_value;
    std::string value;

    maybe_string() : has_value(false), value{} {}
    maybe_string(std::string v) : has_value(true), value(std::move(v)) {}

    bool has() const { return has_value; }

    std::string get() const {
        if (!has_value) throw std::runtime_error("maybe_string is empty");
        return value;
    }
};

// --- maybe_double ---

struct maybe_double {
    bool has_value;
    double value;

    maybe_double() : has_value(false), value{} {}
    maybe_double(double v) : has_value(true), value(v) {}

    bool has() const { return has_value; }

    double get() const {
        if (!has_value) throw std::runtime_error("maybe_double is empty");
        return value;
    }
};

// Imagine also: maybe_connection, maybe_user, maybe_transaction, ...
// Each one is copy-paste with a different type. If you want to add a
// `value_or(default)` method, you have to add it to EVERY struct.

// --- Usage ---

maybe_int find_age(const std::string& name) {
    if (name == "Alice") return maybe_int(30);
    if (name == "Bob")   return maybe_int(25);
    return maybe_int(); // empty
}

maybe_string find_email(const std::string& name) {
    if (name == "Alice") return maybe_string("alice@example.com");
    return maybe_string(); // empty
}

maybe_double find_balance(const std::string& name) {
    if (name == "Alice") return maybe_double(1234.56);
    return maybe_double(); // empty
}

int main() {
    auto age   = find_age("Alice");
    auto email = find_email("Alice");
    auto bal   = find_balance("Bob");

    if (age.has())   std::cout << "Age:     " << age.get()   << "\n";
    if (email.has()) std::cout << "Email:   " << email.get() << "\n";
    if (bal.has())   std::cout << "Balance: " << bal.get()   << "\n";
    else             std::cout << "Balance: not found\n";

    // Try accessing an empty maybe — this throws
    try {
        auto missing = find_email("Charlie");
        std::cout << missing.get() << "\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }
}
