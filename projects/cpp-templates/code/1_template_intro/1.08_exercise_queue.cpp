#include <iostream>
#include <string>
#include <cassert>

// --- Library Code ---

// TODO: parameterize the queue class on the value type

// template <typename T>
class Queue {
    struct Node {
        int value;
        Node* next = nullptr;
    };

    Node* head_;
    Node* tail_;
    int size_;

public:
    Queue() : head_(nullptr), tail_(nullptr), size_(0) {}

    ~Queue() {
        while (head_) {
            Node* tmp = head_;
            head_ = head_->next;
            delete tmp;
        }
    }

public:
    void push(int value) {
        Node* node = new Node{value};
        if (empty()) {
            head_ = tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }
        size_++;
    }

    void pop() {
        assert(!empty());

        Node* old = head_;
        head_ = head_->next;
        delete old;
        size_--;
        if (empty()) tail_ = nullptr;
    }

    int& front() {
        assert(!empty());

        return head_->value;
    }

    const int& front() const {
        assert(!empty());

        return head_->value;
    }

    bool empty() const {
        return size_ == 0;
    }

    int size() const {
        return size_;
    }
};

// --- Tests --- do not modify below this line

/*

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

    // Test 6: owned objects are destroyed on queue destruction
    {
        // increments a counter every time an instance is destroyed
        struct DestroyCounter {
            int& counter_;
            DestroyCounter(int& counter) : counter_{counter} {}
            ~DestroyCounter() { counter_++; }
        };

        int count;
        {
            Queue<DestroyCounter> q2;
            for (int i = 0; i < 100; i++) q2.push(DestroyCounter{count});
            count = 0;
            // Queue destructor called on q2 here
        }

        if (count != 100) { std::cout << "FAIL: objects not all destroyed"; return 1; }

        // If the destructor is wrong, sanitizers will catch the leak.
        passed++;
    }

    std::cout << "All tests passed! (" << passed << "/6)\n";
}

*/