#include <iostream>
#include <utility>


struct intbox {
    intbox(int x) : data_{new int(x)} {
        std::cout << "constructor" << std::endl;
    }
    ~intbox() { 
        delete data_; 
        std::cout << "destructor" << std::endl;
    }

    // these functions obey move and ownership semantics
    intbox(const intbox& other) : data_{new int(*(other.data_))} {
        std::cout << "copy constructor" << std::endl;
    };
    intbox& operator=(const intbox& other) = delete;
    intbox(intbox&& other) : data_{std::exchange(other.data_, nullptr)} {
        std::cout << "move constructor" << std::endl;
    }
    intbox& operator=(intbox&& other) = delete;

    int *data_;
};

intbox get_three() {
    return intbox(3);
}

int main() {
    {
        // temporary immediately destroyed
        get_three();

        // temporary's lifetime is extended
        intbox&& rref = get_three();
        std::cout << "Rvalue reference binds to temporary!" << std::endl;

        // temporary's lifetime is extended
        const intbox& lref = get_three();
        std::cout << "Const lvalue reference binds to temporary!" << std::endl;
    }

    {
        int a = 4;
        int b = 0;
        b = std::move(a);
    }

    std::cout << "Goodbye!" << std::endl;
}
