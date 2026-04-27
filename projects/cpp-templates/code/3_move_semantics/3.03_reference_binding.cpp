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

    // copy constructor
    intbox(const intbox& other) : data_{new int(*(other.data_))} {
        std::cout << "copy constructor" << std::endl;
    };
    intbox& operator=(const intbox& other) = delete;

    // move constructor
    intbox(intbox&& other) : data_{std::exchange(other.data_, nullptr)} {
        std::cout << "move constructor" << std::endl;
    }
    intbox& operator=(intbox&& other) = delete;

    // returns a reference to the data contained
    int& get() {
        return *data_;
    }

    int *data_;

};

void foo(intbox& obj) {
    obj.data_ += 42;
}

void foo(intbox&& obj) {
    obj.data_ = nullptr;
}

int main() {
    {
        std::cout << "Demonstration: extending lifetimes" << std::endl;

        // temporary immediately destroyed after expression it participates in
        intbox(3);

        // temporary's lifetime is extended until scope of rref ends
        intbox&& rref = intbox(3);
        std::cout << "Rvalue reference binds to temporary!" << std::endl;

        // temporary's lifetime is extended until scope of lref ends
        // const lvalue references can bind and extend lifetimes of temporaries
        const intbox& lref = intbox(3);
        std::cout << "Const lvalue reference binds to temporary!" << std::endl;

        // we have seen this in previous examples
        // there is no temporary, the return slot is simply set as the storage of a
        intbox x = intbox(3);
    }

    {
        // tell compiler: we want to extend the tmp's lifetime until end of scope
        intbox&& rref = intbox(3);

        // play around with the temporary
        std::cout << rref.get() << std::endl;
        rref.get() *= 10;
        std::cout << rref.get() << std::endl;
    }

    {
        std::cout << "UNDEFINED BEHAVIOR" << std::endl;
        auto& data = intbox(3).get(); 

        // temporary is destroyed after this expression, leaving a dangling reference
        std::cout << data << std::endl;
    }

    {
        std::cout << "rvalue ref argument" << std::endl;
        foo(intbox(3));
    }

    {
        std::cout << "static dispatch" << std::endl;
        intbox x{3};
        intbox&& rref = std::move(x);

        foo(x); // calls lvalue overload
        foo(rref); // also calls lvalue overload (weird)
        foo(std::move(x)); // calls rvalue overload
        foo(std::move(rref)); // calls rvalue overload
    }

    std::cout << "Goodbye!" << std::endl;
}
