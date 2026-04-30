#include <iostream>
#include <stdexcept>
#include <string>

// --- Library Types ---

struct maybe_int {
    bool has_value_;
    int value_;

    maybe_int() : has_value_(false), value_{} {}
    maybe_int(int v) : has_value_(true), value_(v) {}

    bool has() const { return has_value_; }

    int get() const {
        if (!has_value_) throw std::runtime_error("empty");
        return value_;
    }
};

struct maybe_string {
    bool has_value_;
    std::string value_;

    maybe_string() : has_value_(false), value_{} {}
    maybe_string(std::string v) : has_value_(true), value_(std::move(v)) {}

    bool has() const { return has_value_; }

    std::string get() const {
        if (!has_value_) throw std::runtime_error("empty");
        return value_;
    }
};

struct maybe_double {
    bool has_value_;
    double value_;

    maybe_double() : has_value_(false), value_{} {}
    maybe_double(double v) : has_value_(true), value_(v) {}

    bool has() const { return has_value_; }

    double get() const {
        if (!has_value_) throw std::runtime_error("empty");
        return value_;
    }
};

// --- Database Query Functions ---

maybe_int find_age(const std::string& name) {
    if (name == "Alice") return maybe_int(30);
    if (name == "Bob")   return maybe_int(25);
    return maybe_int();
}

maybe_string find_email(const std::string& name) {
    if (name == "Alice") return maybe_string("alice@example.com");
    return maybe_string();
}

maybe_double find_balance(const std::string& name) {
    if (name == "Alice") return maybe_double(1234.56);
    return maybe_double();
}

// --- Application Code ---

int main() {
    std::string input_name;
    std::cin >> input_name;

    // Fetch user data from database
    maybe_int age = find_age(input_name);
    maybe_string email = find_email(input_name);
    maybe_double bal = find_balance(input_name);

    // Print user data
    if (age.has())   std::cout << "Age:     " << age.get()   << "\n";
    else             std::cout << "Age: not found\n";

    if (email.has()) std::cout << "Email:   " << email.get() << "\n";
    else             std::cout << "Email: not found\n";

    if (bal.has())   std::cout << "Balance: " << bal.get()   << "\n";
    else             std::cout << "Balance: not found\n";
}
