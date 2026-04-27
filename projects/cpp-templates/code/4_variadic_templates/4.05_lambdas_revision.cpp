#include <iostream>

int main() {

    // --- callable ---

    struct callable {
        int operator()(int a, int b) {
            return a + b;
        }
    };

    callable instance;
    std::cout << instance(3, 4) << std::endl;

    // --- lambda ---

    auto lambda = [](int a, int b) {
        return a + b;
    };
    std::cout << lambda(1, 2) << std::endl;

}
