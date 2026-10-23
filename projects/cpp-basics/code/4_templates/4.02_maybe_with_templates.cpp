#include <iostream>
#include <stdexcept>
#include <string>

// --- Library Types ---

template <typename T>
struct Maybe {
    bool mHasValue;
    T mValue;

    Maybe() : mHasValue(false), mValue{} {}
    Maybe(T v) : mHasValue(true), mValue(v) {}

    bool has() const { return mHasValue; }

    const T& get() const {
        if (!mHasValue) throw std::runtime_error("Maybe is empty");
        return mValue;
    }
};

// --- Database Query Functions ---

Maybe<int> findAge(const std::string& name) {
    if (name == "Alice") return Maybe<int>(30);
    if (name == "Bob")   return Maybe<int>(25);
    return Maybe<int>();
}

Maybe<std::string> findEmail(const std::string& name) {
    if (name == "Alice") return Maybe<std::string>("alice@example.com");
    return Maybe<std::string>();
}

Maybe<double> findBalance(const std::string& name) {
    if (name == "Alice") return Maybe<double>(1234.56);
    return Maybe<double>();
}

// --- Application Code ---

int main() {
    auto inputName = std::string{};
    std::cin >> inputName;

    // Fetch user data from database
    auto age = findAge(inputName);
    auto email = findEmail(inputName);
    auto bal = findBalance(inputName);

    // Print user data
    if (age.has())   std::cout << "Age:     " << age.get()   << "\n";
    else             std::cout << "Age: not found\n";

    if (email.has()) std::cout << "Email:   " << email.get() << "\n";
    else             std::cout << "Email: not found\n";

    if (bal.has())   std::cout << "Balance: " << bal.get()   << "\n";
    else             std::cout << "Balance: not found\n";
}
