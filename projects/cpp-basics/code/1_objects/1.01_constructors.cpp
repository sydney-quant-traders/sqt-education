#include <iostream>

struct Gadget {
    int mData[100];

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
};

struct Widget {
    int mData[100];

    Widget() {
        std::cout << "- default constructor" << std::endl;
    }

    ~Widget() {
        std::cout << "- destructor" << std::endl;
    }

    Widget(const Widget& other) {
        std::cout << "- copy constructor" << std::endl;
    }
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

    std::cout << "construction with args examples:" << std::endl;
    {
        auto b = Gadget{67};
    }

    std::cout << "default construction examples:" << std::endl;
    {
        auto c = Gadget{};
    }

    std::cout << "copy construction examples:" << std::endl;
    {
        auto d = a;
        auto e = aLref;
        auto f = aRref; // oddly enough is also a copy construction
    }

    std::cout << "move construction examples:" << std::endl;
    {
        // default constructor is called within getObject()
        // move constructed into return slot (which is b)
        auto g = getObject();
        auto h = std::move(a);
        auto i = std::move(aLref);
        auto j = std::move(aRref);
    }

    std::cout << "assignment operator examples:" << std::endl;
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

    std::cout << "what if no move constructor is defined?" << std::endl;
    {
        auto w = Widget{};
        auto x = std::move(w);
    }

    std::cout << "goodbye" << std::endl;
}