#include <cstdlib>
#include <cstdint>
#include <new>

// --- Library Code ---

struct widget {
    widget(int a, int b) {

    }

};

// stuff
template <typename T>
struct container {
    uint8_t *buffer_;

    container() {
        // allocates storage, does not start any lifetimes
        buffer_ = (uint8_t *) ::operator new(sizeof(T));
    }

    void emplace(int a, int b) {
        // non-allocating placement (aka placement-new)
        new (buffer_) T(a, b);
    }

    void destroy() {
        ~T(buffer_);
    }
};


// --- Application Code ---

