#include <iostream>
#include <vector>

// --- Vtable Types and Forward Declarations---

struct processor;
struct processor_vtable_t {
    void(* process)(processor *, int);
};

extern processor_vtable_t _processor_vtable;
extern processor_vtable_t _printer_vtable;
extern processor_vtable_t _inserter_vtable;

// --- Library Types ---

struct processor {
    processor_vtable_t *vptr_ = &_processor_vtable;

    // calls to process() through object
    void process(int input) {}

    // calls to process() through pointer/reference
    void _virtual_process(int input) {
        auto process_fn_ptr = vptr_->process;
        process_fn_ptr(this, input);
    }

    // static wrapper for process()
    static void _process(processor *this_base, int input) {
        this_base->process(input);
    }
};

struct printer {
    processor base_{&_printer_vtable};

    void process(int input) {
        std::cout << input << " ";
    }

    // static wrapper for process()
    static void _process(processor *this_base, int input) {
        auto _this = (printer *) this_base;
        _this->process(input);
    }
};

struct inserter {
    processor base_{&_inserter_vtable};

    std::vector<int>& dst_;
    inserter(std::vector<int>& dst) : dst_(dst) {}
    void process(int input) {
        dst_.push_back(input);
        dst_.push_back(input);
    }

    // static wrapper for process()
    static void _process(processor *this_base, int input) {
        auto _this = (inserter *) this_base;
        _this->process(input);
    }
};

// --- Code that depends on Base class ---

void process_vector(const std::vector<int>& nums, processor *p) {
    for (auto e : nums) {
        // dynamic dispatch
        p->_virtual_process(e);
    }
}

// --- Application Code ---

int main() {
    std::vector<int> dst;
    auto p = printer{};
    auto d = inserter{dst};

    std::vector<int> src = {1, 2, 3, 4};

    // print all elements of src
    process_vector(src, reinterpret_cast<processor *>(&p));
    // static dispatch
    p.process(6);
    std::cout << "\n";

    // insert all elements of src twice into dst
    process_vector(src, reinterpret_cast<processor *>(&d));
    // static dispatch
    d.process(7);
    for (auto e : dst) {
        std::cout << e << " ";
    }
    std::cout << "\n";

}

// --- Global vtable instances ---

processor_vtable_t _processor_vtable {
    .process = processor::_process,
};

processor_vtable_t _printer_vtable {
    .process = printer::_process,
};

processor_vtable_t _inserter_vtable {
    .process = inserter::_process,
};
