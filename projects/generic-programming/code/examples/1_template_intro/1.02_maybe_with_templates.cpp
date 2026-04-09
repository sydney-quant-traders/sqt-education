#include <iostream>
#include <stdexcept>
#include <string>

// --- Library Types ---

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
};

// --- Database Query Functions ---

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

// --- Application Code ---

int main() {
    std::string input_name;
    std::cin >> input_name;

    // Fetch user data from database
    maybe<int> age = find_age(input_name);
    maybe<std::string> email = find_email(input_name);
    maybe<double> bal = find_balance(input_name);

    // Print user data
    if (age.has())   std::cout << "Age:     " << age.get()   << "\n";
    else             std::cout << "Age: not found\n";

    if (email.has()) std::cout << "Email:   " << email.get() << "\n";
    else             std::cout << "Email: not found\n";

    if (bal.has())   std::cout << "Balance: " << bal.get()   << "\n";
    else             std::cout << "Balance: not found\n";
}
