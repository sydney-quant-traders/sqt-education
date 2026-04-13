#include <memory>
#include <vector>
#include <iostream>
#include <cstdint>
#include <new>

// --- Memory Resource (type-erased abstract base) ---

class memory_resource {
public:
    virtual ~memory_resource() = default;
    virtual void* allocate(size_t bytes, size_t align = alignof(std::max_align_t)) = 0;
    virtual void  deallocate(void* p, size_t bytes, size_t align = alignof(std::max_align_t)) = 0;
    virtual bool  is_equal(const memory_resource& other) const noexcept = 0;
};

// --- Memory Resource Wrapper (type erasure layer) ---

template <typename Resource>
class memory_resource_wrapper : public memory_resource {
public:
    template <typename... Args>
    explicit memory_resource_wrapper(Args&&... args)
        : resource_(std::forward<Args>(args)...) {}

private:
    void* allocate(size_t bytes, size_t align) override {
        return resource_.allocate(bytes, align);
    }

    void deallocate(void* p, size_t bytes, size_t align) override {
        resource_.deallocate(p, bytes, align);
    }

    bool is_equal(const memory_resource& other) const noexcept override {
        auto* o = dynamic_cast<const memory_resource_wrapper*>(&other);
        return o && &resource_ == &o->resource_;
    }

    Resource resource_;
};

// --- Malloc Resource ---

class malloc_resource {
public:
    void* allocate(size_t bytes, size_t align) {
        void* p = std::aligned_alloc(align, (bytes + align - 1) & ~(align - 1));
        if (!p) throw std::bad_alloc{};
        std::cout << "malloc_resource: alloc " << bytes << " bytes" << std::endl;
        return p;
    }

    void deallocate(void* p, size_t bytes, size_t) {
        std::cout << "malloc_resource: dealloc " << bytes << " bytes" << std::endl;
        std::free(p);
    }
};

// --- Free List Resource ---

class free_list_resource {
private:
    struct free_header {
        size_t size;
        free_header *next;
        free_header(size_t size, free_header *next) : size(size), next(next) {}
    };

    struct alloc_header {
        size_t size;
        alloc_header(size_t size) : size(size) {}
    };

public:
    free_list_resource(void* buf, size_t capacity) {
        free_ = std::construct_at<free_header>(static_cast<free_header *>(buf), capacity, nullptr);
    }

    void* allocate(size_t bytes, size_t /*align*/) {
        // account for alloc_header and alignment
        size_t req_size = std::max(bytes + sizeof(alloc_header), sizeof(free_header));
        req_size = (req_size + alignof(free_header) - 1) & ~(alignof(free_header) - 1);

        // find the first fit
        auto curr = free_;
        free_header *prev = nullptr;
        while (curr != nullptr) {
            if (curr->size >= req_size) break;
            prev = curr;
            curr = curr->next;
        }
        if (!curr) throw std::bad_alloc{};

        // determine if chunk should be split
        free_header *next_free;
        const size_t MIN_THRESH = sizeof(free_header);
        size_t remaining = curr->size - req_size;
        if (remaining >= MIN_THRESH) {
            next_free = std::construct_at<free_header>(reinterpret_cast<free_header *>(reinterpret_cast<char *>(curr) + req_size), remaining, curr->next);
            curr->size = req_size;
        } else {
            next_free = curr->next;
        }

        // remove allocated chunk from freelist
        if (prev) {
            prev->next = next_free;
        } else {
            free_ = next_free;
        }

        // now curr points to the allocated chunk that has been removed from the list
        auto chunk_size = curr->size;
        std::destroy_at<free_header>(curr);
        auto alloc_chunk = reinterpret_cast<alloc_header *>(curr);
        std::construct_at<alloc_header>(alloc_chunk, chunk_size);
        std::cout << "free_list_resource: alloc " << bytes << " bytes" << std::endl;
        return reinterpret_cast<char *>(alloc_chunk) + sizeof(alloc_header);
    }

    void deallocate(void* ptr, size_t bytes, size_t) {
        std::cout << "free_list_resource: dealloc " << bytes << " bytes" << std::endl;

        // find last free node with address before ptr
        auto curr = free_;
        free_header *prev = nullptr;
        while (curr != nullptr) {
            if (reinterpret_cast<uintptr_t>(curr) >= reinterpret_cast<uintptr_t>(ptr)) break;
            prev = curr;
            curr = curr->next;
        }

        // extract information from the alloc header
        auto alloc_chunk = reinterpret_cast<alloc_header *>(static_cast<char *>(ptr) - sizeof(alloc_header));
        auto chunk_size = alloc_chunk->size;

        // replace alloc header with free header, and insert into free list
        std::destroy_at<alloc_header>(alloc_chunk);
        free_header *freed_chunk = std::construct_at<free_header>(reinterpret_cast<free_header *>(alloc_chunk), chunk_size, nullptr);
        if (prev) {
            freed_chunk->next = prev->next;
            prev->next = freed_chunk;
        } else {
            freed_chunk->next = free_;
            free_ = freed_chunk;
        }

        // edge case: free the entire buffer
        if (!prev && !curr) return;

        // attempt coalesce with previous
        auto coalesce_attempt = freed_chunk;
        if (
            prev &&
            reinterpret_cast<char *>(prev) + prev->size == reinterpret_cast<char *>(coalesce_attempt)
        ) {
            prev->size = prev->size + coalesce_attempt->size;
            prev->next = coalesce_attempt->next;
            coalesce_attempt = prev;
        }

        // attempt coalesce with next
        if (
            coalesce_attempt->next &&
            reinterpret_cast<char *>(coalesce_attempt) + coalesce_attempt->size == reinterpret_cast<char *>(coalesce_attempt->next)
        ) {
            coalesce_attempt->size = coalesce_attempt->size + coalesce_attempt->next->size;
            coalesce_attempt->next = coalesce_attempt->next->next;
        }
    }

private:
    free_header *free_;
};

// --- Polymorphic Allocator (concrete value type) ---

template <typename T>
class polymorphic_allocator {
public:
    using value_type = T;

    polymorphic_allocator(memory_resource* r) noexcept : resource_(r) {}

    template <typename U>
    polymorphic_allocator(const polymorphic_allocator<U>& other) noexcept
        : resource_(other.resource()) {}

    T* allocate(size_t n) {
        return static_cast<T*>(resource_->allocate(n * sizeof(T), alignof(T)));
    }

    void deallocate(T* p, size_t n) noexcept {
        resource_->deallocate(p, n * sizeof(T), alignof(T));
    }

    memory_resource* resource() const noexcept { return resource_; }

    template <typename U>
    bool operator==(const polymorphic_allocator<U>& other) const noexcept {
        return resource_->is_equal(*other.resource());
    }

    template <typename U>
    bool operator!=(const polymorphic_allocator<U>& other) const noexcept {
        return !(*this == other);
    }

private:
    memory_resource* resource_;
};

// --- Application Code ---

int main() {
    memory_resource_wrapper<malloc_resource> malloc_res;
    alignas(16) char buf[4096];
    memory_resource_wrapper<free_list_resource> fl_res{buf, sizeof(buf)};

    // Same allocator type, different underlying resources
    polymorphic_allocator<int> a1{&malloc_res};
    polymorphic_allocator<int> a2{&fl_res};

    // Can use with std::vector — polymorphic_allocator is a proper value type
    std::vector<int, polymorphic_allocator<int>> v1(a1);
    std::vector<int, polymorphic_allocator<int>> v2(a2);

    for (auto* v : {&v1, &v2}) {
        v->push_back(10);
        v->push_back(20);
        v->push_back(30);
        for (int x : *v) std::cout << x << " ";
        std::cout << std::endl;
    }
}
