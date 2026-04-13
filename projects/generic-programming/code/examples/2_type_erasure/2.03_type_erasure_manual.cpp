#include <iostream>

#include <list>
#include <deque>

// --- Polymorphic Wrappers ---

class base {
public:
    virtual void push_back(int val) = 0;
    virtual void pop_front() = 0;
    virtual int front() = 0;
};

// wrapper around a list
class list_wrapper : public base {
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
    std::list<int> elems_;
};

// wrapper around a deque
class deque_wrapper : public base {
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
    std::deque<int> elems_;
};

// --- Application Code ---

int main() {
    std::cout << "Do you like deques more than lists?" << std::endl;
    bool likes_deques_more;
    std::cin >> likes_deques_more;

    // choose the underlying container based on a run-time condition
    base *q = likes_deques_more
        ? (base *) new deque_wrapper{}
        : (base *) new list_wrapper{};

    // perform some operations on this queue
    q->push_back(67);
    auto x = q->front();
    q->pop_front();
}