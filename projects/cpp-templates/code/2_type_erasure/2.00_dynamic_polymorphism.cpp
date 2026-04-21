#include <iostream>
#include <vector>

// --- Library Types ---

struct processor {
    virtual void process(int input) {};
};

struct printer : public processor {
    void process(int input) override {
        std::cout << input << " ";
    }
};

struct double_inserter : public processor {
    std::vector<int>& dst_;
    double_inserter(std::vector<int>& dst) : dst_(dst) {}
    void process(int input) override {
        dst_.push_back(input);
        dst_.push_back(input);
    }
};

// --- Code that depends on Base class ---

void process_vector(const std::vector<int>& nums, processor *p) {
    for (auto e : nums) {
        // dynamic dispatch
        p->process(e);
    }
}

// --- Application Code ---

int main() {
    std::vector<int> dst;
    auto p = printer{};
    auto d = double_inserter{dst};

    std::vector<int> src = {1, 2, 3, 4};

    // print all elements of src
    process_vector(src, &p);
    // static dispatch
    p.process(6);
    std::cout << "\n";

    // insert all elements of src twice into dst
    process_vector(src, &d);
    // static dispatch
    d.process(7);
    for (auto e : dst) {
        std::cout << e << " ";
    }
    std::cout << "\n";

}
