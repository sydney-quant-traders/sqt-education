#include <utility>

// --- Library Code ---

template <typename T>
class unique_ptr {
public:
    unique_ptr(T *data) : data_{data} {}
    ~unique_ptr() {
        delete data_;
    }

    unique_ptr(const unique_ptr& other) = delete;
    unique_ptr& operator=(const unique_ptr& other) = delete;
    unique_ptr(unique_ptr&& other) : data_{std::exchange(other.data_, nullptr)} {}
    unique_ptr& operator=(unique_ptr&& other) {
        std::swap(data_, other.data_);
        return *this;
    }

    T *get() {
        return data_;
    }
    
private:
    T *data_;

};

// TODO: implement templated make unique
// unique_ptr<T> make_unique(...) {...}

// --- Application Code ---

struct widget {
    widget(int a, int& b, int&& c) {}
};

unique_ptr<widget> make_unique_widget(int a, int& b, int&& c) {
    return unique_ptr<widget>(new widget(a, b, std::move(c)));
}

int main() {
    int x = 5;
    unique_ptr<widget> w1 = make_unique_widget(6, x, 8);
    auto w2 = std::move(w1);

}
