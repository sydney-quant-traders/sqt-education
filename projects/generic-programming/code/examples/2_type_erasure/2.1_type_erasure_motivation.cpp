#include <iostream>

// Note that both std::list and std::deque have the methods
// [push_back, pop_front, front]
#include <list>
#include <deque>

int main() {
    // choose the underlying container based on a run-time condition
    // this will not compile since deque and list don't share a base class
    std::cout << "Do you like deques more than lists?" << std::endl;
    bool likes_deques_more;
    std::cin >> likes_deques_more;
    auto q = likes_deques_more
        ? new std::deque<int>{}
        : new std::list<int>{};

    // perform some operations on this queue
    q->push_back(67);
    auto x = q->front();
    q->pop_front();
}