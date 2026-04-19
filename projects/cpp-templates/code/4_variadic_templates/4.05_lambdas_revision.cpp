#include <iostream>

int main() {
    auto cmp = [](int a, int b){
        return a < b;
    };

    std::cout << cmp(1, 2) << std::endl;
}