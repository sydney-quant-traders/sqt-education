#include <iostream>
#include <deque>
#include <list>
#include <cassert>

// --- Library Code ---

// Requirements:
// - ContainerTemplate is a template class that can take Value as a template parameter
// - i.e. ContainerTemplate<Value> is a valid type
// - The resulting type must have appropriate push_back(), pop_front(), and front() methods

template <template <typename> typename ContainerTemplate, typename Value>
void push(ContainerTemplate<Value>& c, Value val) {
    c.push_back(val);
    std::cout << "Pushed: " << val << "\n";
}

template <template <typename> typename ContainerTemplate, typename Value>
Value pop(ContainerTemplate<Value>& c) {
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
    pop<std::deque, int>(d);
    pop<std::deque, int>(d);

    // <Container, Value> = <list<char>, char>
    std::list<char> l;
    push(l, 'S');
    push(l, 'Q');
    push(l, 'T');
    pop<std::list, char>(l);
    pop<std::list, char>(l);
    pop<std::list, char>(l);
}
    