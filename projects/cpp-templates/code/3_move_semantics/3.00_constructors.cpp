#include <iostream>

struct gadget {
    gadget(int dummy) {
        std::cout << "- constructor with args" << std::endl;
    }

    gadget() {
        std::cout << "- default constructor" << std::endl;
    }

    ~gadget() {
        std::cout << "- destructor" << std::endl;
    }

    gadget(const gadget& other) {
        std::cout << "- copy constructor" << std::endl;
    }

    gadget& operator=(const gadget& other) {
        std::cout << "- copy assignment operator" << std::endl;
        return *this;
    }

    gadget(gadget&& other) {
        std::cout << "- move constructor" << std::endl;
    }

    gadget& operator=(gadget&& other) {
        std::cout << "- move assignment operator" << std::endl;
        return *this;
    }

    int data_[100];
};

struct widget {
    widget() {
        std::cout << "- default constructor" << std::endl;
    }

    ~widget() {
        std::cout << "- destructor" << std::endl;
    }

    widget(const widget& other) {
        std::cout << "- copy constructor" << std::endl;
    }

    int data_[100];
};


// return value from method is rvalue
// disable NVRO pls
gadget get_object() {
    gadget o{};
    return o;
}

int main() {
    gadget a{};
    gadget& a_lref = a;
    gadget&& a_rref = std::move(a);

    {
        std::cout << "construction with args examples:" << std::endl;

        gadget c(42);
        gadget d{42};

        // equivalent to c, d
        auto e = gadget(42);
        auto f = gadget{42};
    }

    {
        std::cout << "default construction examples:" << std::endl;

        gadget b;
        // object c(); // doesn't work because of most vexing parse
        gadget d{};
        auto e = gadget();
        auto f = gadget{};
    }

    {
        std::cout << "copy construction examples:" << std::endl;

        gadget c{a};
        gadget d{a_lref};
        // this one may be counterintuitive
        gadget e{a_rref};

        // equivalent to c, d, e
        auto f = a;
        auto g = a_lref;
        auto h = a_rref;
    }


    {
        std::cout << "move construction examples:" << std::endl;

        // default constructor is called within get_object()
        // move constructed into return slot (which is b)
        gadget b{get_object()};

        gadget c{std::move(a)};
        gadget d{std::move(a_lref)};
        gadget e{std::move(a_rref)};

        // equivalent to c, d, e
        auto f = std::move(a);
        auto g = std::move(a_lref);
        auto h = std::move(a_rref);
    }

    {
        std::cout << "constructing b:" << std::endl;
        gadget b;

        std::cout << "copy assignment operator examples:" << std::endl;
        b = a;
        b = a_lref;
        b = a_rref;

        std::cout << "copy assignment operator examples:" << std::endl;
        b = std::move(a);
        b = std::move(a_lref);
        b = std::move(a_rref);
    }

    {
        std::cout << "what if no move constructor is defined?" << std::endl;

        widget w;
        widget x{std::move(w)};
    }

    std::cout << "goodbye" << std::endl;
}