#include <iostream>

#include <list>
#include <deque>

// --- Polymorphic Wrappers ---

class base {
public:
    virtual void push_back(int val) = 0;
    virtual void pop_front() = 0;
    virtual int front() = 0;
    virtual ~base();
};

// wrapper around a list
template <typename Container>
class wrapper : public base {
public:
    void push_back(int val) override {
        elems_.push_back(val);
    }

    void pop_front() override {
        elems_.pop_front();
    }

    int front() override {
        return elems_.front();
    }

private: 
    Container elems_;
};

// --- Application Code ---

int main() {
    std::cout << "Do you like deques more than lists?" << std::endl;
    bool likes_deques_more;
    std::cin >> likes_deques_more;

    // choose the underlying container based on a run-time condition
    base *q = likes_deques_more
        ? (base *) new wrapper<std::deque<int>>{}
        : (base *) new wrapper<std::list<int>>{};

    // perform some operations on this queue
    q->push_back(67);
    auto x = q->front();
    q->pop_front();

    delete q;
}