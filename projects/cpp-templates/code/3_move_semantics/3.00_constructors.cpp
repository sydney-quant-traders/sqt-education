#include <iostream>

struct object {
    object(int dummy) {
        std::cout << "- constructor with args" << std::endl;
    }

    object() {
        std::cout << "- default constructor" << std::endl;
    }

    ~object() {
        std::cout << "- destructor" << std::endl;
    }

    object(const object& other) {
        std::cout << "- copy constructor" << std::endl;
    }

    object& operator=(const object& other) {
        std::cout << "- copy assignment operator" << std::endl;
        return *this;
    }

    object(object&& other) {
        std::cout << "- move constructor" << std::endl;
    }

    object& operator=(object&& other) {
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
object get_object() {
    object o{};
    return o;
}

int main() {
    object a{};
    object& a_lref = a;
    object&& a_rref = std::move(a);

    {
        std::cout << "construction with args examples:" << std::endl;

        object c(42);
        object d{42};

        // equivalent to c, d
        auto e = object(42);
        auto f = object{42};
    }

    {
        std::cout << "default construction examples:" << std::endl;

        object b;
        // object c(); // doesn't work because of most vexing parse
        object d{};
        auto e = object();
        auto f = object{};
    }

    {
        std::cout << "copy construction examples:" << std::endl;

        object c{a};
        object d{a_lref};
        // this one may be counterintuitive
        object e{a_rref};

        // equivalent to c, d, e
        auto f = a;
        auto g = a_lref;
        auto h = a_rref;
    }


    {
        std::cout << "move construction examples:" << std::endl;

        // default constructor is called within get_object()
        // move constructed into return slot (which is b)
        object b{get_object()};

        object c{std::move(a)};
        object d{std::move(a_lref)};
        object e{std::move(a_rref)};

        // equivalent to c, d, e
        auto f = std::move(a);
        auto g = std::move(a_lref);
        auto h = std::move(a_rref);
    }

    {
        std::cout << "constructing b:" << std::endl;
        object b;

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