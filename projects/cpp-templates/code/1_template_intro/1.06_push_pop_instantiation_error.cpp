#include <iostream>
#include <deque>
#include <list>
#include <cassert>

// --- Library Code ---

template <typename Container, typename Value>
void push(Container& c, Value val) {
    c.push_back(val);
    std::cout << "Pushed: " << val << "\n";
}

template <typename Container, typename Value>
Value pop(Container& c) {
    Value result = c.front();
    c.pop_front();
    std::cout << "Popped: " << result << "\n";
    return result;
}

// --- Application Code ---

int main() {
    // <Container, Value> = <deque<int>, int>
    std::deque<int> d;
    push(d, 4);
    push(d, 2);
    pop<std::deque<int>, int>(d);
    pop<std::deque<int>, int>(d);

    // <Container, Value> = <list<char>, char>
    std::list<char> l;
    push(l, 'S');
    push(l, 'Q');
    push(l, 'T');
    pop<std::list<char>, char>(l);
    pop<std::list<char>, char>(l);
    pop<std::list<char>, char>(l);

    // ERROR: <Container, Value> = <double, int>
    // ERROR: double has no .pop_front() method
    /*
    double x;
    push(x, 1);
    */
}
    