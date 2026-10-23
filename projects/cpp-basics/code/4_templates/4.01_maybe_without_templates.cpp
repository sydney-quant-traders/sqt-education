#include <iostream>
#include <stdexcept>
#include <string>

// --- Library Types ---

struct MaybeInt {
    bool mHasValue;
    int mValue;

    MaybeInt() : mHasValue(false), mValue{} {}
    MaybeInt(int v) : mHasValue(true), mValue(v) {}

    bool has() const { return mHasValue; }

    int get() const {
        if (!mHasValue) throw std::runtime_error("empty");
        return mValue;
    }
};

struct MaybeString {
    bool mHasValue;
    std::string mValue;

    MaybeString() : mHasValue(false), mValue{} {}
    MaybeString(std::string v) : mHasValue(true), mValue(std::move(v)) {}

    bool has() const { return mHasValue; }

    std::string get() const {
        if (!mHasValue) throw std::runtime_error("empty");
        return mValue;
    }
};

struct MaybeDouble {
    bool mHasValue;
    double mValue;

    MaybeDouble() : mHasValue(false), mValue{} {}
    MaybeDouble(double v) : mHasValue(true), mValue(v) {}

    bool has() const { return mHasValue; }

    double get() const {
        if (!mHasValue) throw std::runtime_error("empty");
        return mValue;
    }
};

// --- Database Query Functions ---

MaybeInt findAge(const std::string& name) {
    if (name == "Alice") return MaybeInt(30);
    if (name == "Bob")   return MaybeInt(25);
    return MaybeInt();
}

MaybeString findEmail(const std::string& name) {
    if (name == "Alice") return MaybeString("alice@example.com");
    return MaybeString();
}

MaybeDouble findBalance(const std::string& name) {
    if (name == "Alice") return MaybeDouble(1234.56);
    return MaybeDouble();
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
