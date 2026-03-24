// exercise_queue.cpp — After Part 1 (Templates)
//
// EXERCISE: Implement a generic Queue<T> backed by a singly linked list.
//
// The Queue supports:
//   - push(value)  — add an element to the back
//   - pop()        — remove the element at the front
//   - front()      — return a reference to the front element
//   - empty()      — return true if the queue has no elements
//   - size()       — return the number of elements
//
// The queue must properly clean up its memory when destroyed.
//
// YOUR TASK: Fill in every spot marked TODO. Once you're done,
// the code should compile and print "All tests passed!".
//
// Compile: g++ -std=c++17 -o exercise_queue exercise_queue.cpp
//
// Hints:
//   - A singly linked list node holds a value and a pointer to the next node.
//   - push() appends to the tail, pop() removes from the head.
//   - Keep a head pointer, a tail pointer, and a size counter.

#include <iostream>
#include <stdexcept>
#include <string>

template <typename T>
struct Node {
    T value;
    Node* next;

    Node(T v) : value(std::move(v)), next(nullptr) {}
};

template <typename T>
class Queue {
    Node<T>* head_;
    Node<T>* tail_;
    int size_;

public:
    Queue() : head_(nullptr), tail_(nullptr), size_(0) {}

    ~Queue() {
        // TODO: Walk the linked list and delete every node.
    }

    void push(T value) {
        // TODO: Create a new node. If the queue is empty, it becomes
        // both head and tail. Otherwise, append it after the current tail.
        // Don't forget to increment size_.
    }

    void pop() {
        if (empty()) throw std::runtime_error("pop on empty queue");

        // TODO: Save the current head, advance head to head->next,
        // and delete the old head. If the queue is now empty, reset
        // tail to nullptr. Don't forget to decrement size_.
    }

    T& front() {
        if (empty()) throw std::runtime_error("front on empty queue");

        // TODO: Return a reference to the head node's value.
    }

    const T& front() const {
        if (empty()) throw std::runtime_error("front on empty queue");

        // TODO: Return a const reference to the head node's value.
    }

    bool empty() const {
        // TODO: Return true if the queue has no elements.
    }

    int size() const {
        // TODO: Return the number of elements in the queue.
    }
};

// ============================================================
// Tests — do not modify below this line
// ============================================================

int main() {
    int passed = 0;

    // Test 1: empty queue
    {
        Queue<int> q;
        if (!q.empty()) { std::cout << "FAIL: new queue should be empty\n"; return 1; }
        if (q.size() != 0) { std::cout << "FAIL: new queue size should be 0\n"; return 1; }
        passed++;
    }

    // Test 2: push and front
    {
        Queue<int> q;
        q.push(10);
        q.push(20);
        q.push(30);
        if (q.front() != 10) { std::cout << "FAIL: front should be 10\n"; return 1; }
        if (q.size() != 3) { std::cout << "FAIL: size should be 3\n"; return 1; }
        passed++;
    }

    // Test 3: pop
    {
        Queue<int> q;
        q.push(1);
        q.push(2);
        q.push(3);
        q.pop();
        if (q.front() != 2) { std::cout << "FAIL: front after pop should be 2\n"; return 1; }
        if (q.size() != 2) { std::cout << "FAIL: size after pop should be 2\n"; return 1; }
        q.pop();
        q.pop();
        if (!q.empty()) { std::cout << "FAIL: queue should be empty after popping all\n"; return 1; }
        passed++;
    }

    // Test 4: works with strings (proves it's generic)
    {
        Queue<std::string> q;
        q.push("hello");
        q.push("world");
        if (q.front() != "hello") { std::cout << "FAIL: string front should be hello\n"; return 1; }
        q.pop();
        if (q.front() != "world") { std::cout << "FAIL: string front should be world\n"; return 1; }
        passed++;
    }

    // Test 5: push after emptying
    {
        Queue<int> q;
        q.push(1);
        q.pop();
        q.push(2);
        if (q.front() != 2) { std::cout << "FAIL: front after re-push should be 2\n"; return 1; }
        if (q.size() != 1) { std::cout << "FAIL: size after re-push should be 1\n"; return 1; }
        passed++;
    }

    // Test 6: exception on empty
    {
        Queue<int> q;
        bool caught = false;
        try { q.pop(); } catch (const std::runtime_error&) { caught = true; }
        if (!caught) { std::cout << "FAIL: pop on empty should throw\n"; return 1; }
        caught = false;
        try { q.front(); } catch (const std::runtime_error&) { caught = true; }
        if (!caught) { std::cout << "FAIL: front on empty should throw\n"; return 1; }
        passed++;
    }

    // Test 7: destructor doesn't leak (test with large queue)
    {
        Queue<int> q;
        for (int i = 0; i < 10000; i++) q.push(i);
        // If the destructor is wrong, sanitizers will catch the leak.
        passed++;
    }

    std::cout << "All tests passed! (" << passed << "/7)\n";
}
