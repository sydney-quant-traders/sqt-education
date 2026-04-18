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

    intbox(const intbox& other) : data_{new int(*(other.data_))} {
        std::cout << "copy constructor" << std::endl;
    };
    intbox& operator=(const intbox& other) = delete;
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

intbox get_three() {
    // construct `a` in this stack frame
    intbox a{3};
    // move construct `a` into the return slot (provided NVRO is disabled)
    return a;
}

int main() {
    {
        std::cout << "Demonstration: extending lifetimes" << std::endl;

        // temporary immediately destroyed after expression it participates in
        get_three();

        // temporary's lifetime is extended until scope of rref ends
        intbox&& rref = get_three();
        std::cout << "Rvalue reference binds to temporary!" << std::endl;

        // temporary's lifetime is extended until scope of lref ends
        // const lvalue references can bind and extend lifetimes of temporaries
        const intbox& lref = get_three();
        std::cout << "Const lvalue reference binds to temporary!" << std::endl;

        // we have seen this in previous examples
        // there is no temporary, the return slot is simply set as the storage of a
        intbox x = get_three();
    }

    {
        // tell the compiler we want to extend the lifetime of this temporary until the end of this scope
        intbox&& rref = get_three();

        // play around with the temporary
        std::cout << rref.get() << std::endl;
        rref.get() *= 10;
        std::cout << rref.get() << std::endl;
    }

    {
        std::cout << "UNDEFINED BEHAVIOR" << std::endl;
        auto& data = get_three().get(); 

        // temporary is destroyed after this expression, leaving a dangling reference
        std::cout << data << std::endl;
    }

    std::cout << "Goodbye!" << std::endl;
}
