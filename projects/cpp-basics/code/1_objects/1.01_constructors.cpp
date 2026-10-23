#include <iostream>

struct Gadget {
    Gadget(int dummy) {
        std::cout << "- constructor with args" << std::endl;
    }

    Gadget() {
        std::cout << "- default constructor" << std::endl;
    }

    ~Gadget() {
        std::cout << "- destructor" << std::endl;
    }

    Gadget(const Gadget& other) {
        std::cout << "- copy constructor" << std::endl;
    }

    Gadget& operator=(const Gadget& other) {
        std::cout << "- copy assignment operator" << std::endl;
        return *this;
    }

    Gadget(Gadget&& other) {
        std::cout << "- move constructor" << std::endl;
    }

    Gadget& operator=(Gadget&& other) {
        std::cout << "- move assignment operator" << std::endl;
        return *this;
    }

    int mData[100];
};

struct Widget {
    Widget() {
        std::cout << "- default constructor" << std::endl;
    }

    ~Widget() {
        std::cout << "- destructor" << std::endl;
    }

    Widget(const Widget& other) {
        std::cout << "- copy constructor" << std::endl;
    }

    int mData[100];
};


// return value from method is rvalue
// disable NVRO pls
Gadget getObject() {
    auto o = Gadget{};
    return o;
}

int main() {
    auto a = Gadget{};
    auto& aLref = a;
    auto&& aRref = std::move(a);

    {
        std::cout << "construction with args examples:" << std::endl;

        Gadget c(42);
        Gadget d{42};

        // equivalent to c, d
        auto e = Gadget(42);
        auto f = Gadget{42};
    }

    {
        std::cout << "default construction examples:" << std::endl;

        Gadget b;
        // object c(); // doesn't work because of most vexing parse
        Gadget d{};
        auto e = Gadget();
        auto f = Gadget{};
    }

    {
        std::cout << "copy construction examples:" << std::endl;

        Gadget c{a};
        Gadget d{aLref};
        // this one may be counterintuitive
        Gadget e{aRref};

        // equivalent to c, d, e
        auto f = a;
        auto g = aLref;
        auto h = aRref;
    }


    {
        std::cout << "move construction examples:" << std::endl;

        // default constructor is called within getObject()
        // move constructed into return slot (which is b)
        auto b = Gadget{getObject()};

        Gadget c{std::move(a)};
        Gadget d{std::move(aLref)};
        Gadget e{std::move(aRref)};

        // equivalent to c, d, e
        auto f = std::move(a);
        auto g = std::move(aLref);
        auto h = std::move(aRref);
    }

    {
        std::cout << "constructing b:" << std::endl;
        auto b = Gadget{};

        std::cout << "copy assignment operator examples:" << std::endl;
        b = a;
        b = aLref;
        b = aRref;

        std::cout << "move assignment operator examples:" << std::endl;
        b = std::move(a);
        b = std::move(aLref);
        b = std::move(aRref);
    }

    {
        std::cout << "what if no move constructor is defined?" << std::endl;

        auto w = Widget{};
        auto x = Widget{std::move(w)};
    }

    std::cout << "goodbye" << std::endl;
}