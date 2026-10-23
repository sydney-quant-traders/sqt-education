#include <iostream>
#include <utility>

struct IntBox {
    IntBox(int x) : mData{new int(x)} {
        std::cout << "constructor" << std::endl;
    }
    ~IntBox() { 
        delete mData; 
        std::cout << "destructor" << std::endl;
    }

    // copy constructor
    IntBox(const IntBox& other) : mData{new int(*(other.mData))} {
        std::cout << "copy constructor" << std::endl;
    };
    IntBox& operator=(const IntBox& other) = delete;

    // move constructor
    IntBox(IntBox&& other) : mData{std::exchange(other.mData, nullptr)} {
        std::cout << "move constructor" << std::endl;
    }
    IntBox& operator=(IntBox&& other) = delete;

    // returns a reference to the data contained
    int& get() {
        return *mData;
    }

    int *mData;

};

void foo(IntBox& obj) {
    obj.mData += 42;
}

void foo(IntBox&& obj) {
    obj.mData = nullptr;
}

int main() {
    {
        std::cout << "Demonstration: extending lifetimes" << std::endl;

        // temporary immediately destroyed after expression it participates in
        IntBox{3};

        // temporary's lifetime is extended until scope of rref ends
        auto&& rref = IntBox{3};
        std::cout << "Rvalue reference binds to temporary!" << std::endl;

        // temporary's lifetime is extended until scope of lref ends
        // const lvalue references can bind and extend lifetimes of temporaries
        const auto& lref = IntBox{3};
        std::cout << "Const lvalue reference binds to temporary!" << std::endl;

        // we have seen this in previous examples
        // there is no temporary, the return slot is simply set as the storage of a
        auto x = IntBox{3};
    }

    {
        // tell compiler: we want to extend the tmp's lifetime until end of scope
        auto&& rref = IntBox{3};

        // play around with the temporary
        std::cout << rref.get() << std::endl;
        rref.get() *= 10;
        std::cout << rref.get() << std::endl;
    }

    {
        std::cout << "UNDEFINED BEHAVIOR" << std::endl;
        auto& data = IntBox{3}.get(); 

        // temporary is destroyed after this expression, leaving a dangling reference
        std::cout << data << std::endl;
    }

    {
        std::cout << "rvalue ref argument" << std::endl;
        foo(IntBox{3});
    }

    {
        std::cout << "static dispatch" << std::endl;
        auto x = IntBox{3};
        auto&& rref = std::move(x);

        foo(x); // calls lvalue overload
        foo(rref); // also calls lvalue overload (weird)
        foo(std::move(x)); // calls rvalue overload
        foo(std::move(rref)); // calls rvalue overload
    }

    std::cout << "Goodbye!" << std::endl;
}
