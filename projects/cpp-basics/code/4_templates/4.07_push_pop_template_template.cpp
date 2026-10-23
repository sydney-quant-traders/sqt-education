#include <iostream>
#include <deque>
#include <list>
#include <cassert>

// --- Library Code ---

// Requirements:
// - ContainerTemplate is a template class that can take Value and an Allocator as template parameters
// - i.e. ContainerTemplate<Value, Allocator> is a valid type (e.g. std::deque, std::list)
// - The resulting type must have appropriate push_back(), pop_front(), and front() methods

template <template <typename, typename> typename ContainerTemplate, typename Value, typename Allocator>
void push(ContainerTemplate<Value, Allocator>& c, Value val) {
    c.push_back(val);
    std::cout << "Pushed: " << val << "\n";
}

template <template <typename, typename> typename ContainerTemplate, typename Value, typename Allocator>
Value pop(ContainerTemplate<Value, Allocator>& c) {
    auto result = c.front();
    c.pop_front();
    std::cout << "Popped: " << result << "\n";
    return result;
}

// --- Application Code ---

int main() {
    // <Container, Value> = <deque<int>, int>
    auto d = std::deque<int>{};
    push(d, 4);
    push(d, 2);
    pop<std::deque, int>(d);
    pop<std::deque, int>(d);

    // <Container, Value> = <list<char>, char>
    auto l = std::list<char>{};
    push(l, 'S');
    push(l, 'Q');
    push(l, 'T');
    pop<std::list, char>(l);
    pop<std::list, char>(l);
    pop<std::list, char>(l);
}
    