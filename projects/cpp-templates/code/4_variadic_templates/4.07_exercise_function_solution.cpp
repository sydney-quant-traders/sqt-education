// Given this non-generic code
// Modify to make it generic

#include <memory>
#include <map>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

// --- Function Library Code ---

namespace detail {

template <typename Ret, typename... Args>
struct type_erased_callable {
    virtual ~type_erased_callable() {}
    virtual Ret operator()(Args... args) = 0;
};

template <typename T, typename Ret, typename... Args>
class callable_wrapper : public type_erased_callable<Ret, Args...> {
public:
    template <typename F>
    callable_wrapper(F&& callable) : callable_{std::forward<F>(callable)} {}

    Ret operator()(Args... args) override {
        return callable_(std::forward<Args>(args)...);
    }

private:
    T callable_;

};

} // namespace detail

template <typename Ret, typename... Args>
class function {
public:
    template <typename T> // T is a callable
    function(T&& callable) 
    : impl_{std::make_unique<detail::callable_wrapper<std::decay_t<T>, Ret, Args...>>(std::forward<T>(callable))} 
    {}

    function(const function& other) = delete;
    function& operator=(const function& other) = delete;

    function(function&& other) = default;
    function& operator=(function&& other) = default;

public:
    Ret operator()(Args... args) {
        return impl_->operator()(std::forward<Args>(args)...);
    }

private:
    std::unique_ptr<detail::type_erased_callable<Ret, Args...>> impl_;

};

// --- Scheduler Library Code ---

class scheduler {
public:
    using clock = std::chrono::steady_clock;
    using time_point = clock::time_point;

    void schedule(function<bool, int&> handler, std::chrono::milliseconds delay) {
        work_.emplace(clock::now() + delay, std::move(handler));
    }

    void run() {
        while (!work_.empty()) {
            auto it = work_.begin();
            if (clock::now() >= it->first) {
                int error_code;
                // invoke the function object
                it->second(error_code);
                work_.erase(it);
            }
        }
    }

private:
    std::map<time_point, function<bool, int&>> work_;

};

// --- Application Code ---

struct timer {
    timer (scheduler *sched, std::chrono::milliseconds interval) : sched_{sched}, interval_{interval} {}

    bool operator()(int& ec) {
        std::cout << "timer tick: " << interval_ << std::endl;
        timer copy = *this;
        sched_->schedule(std::move(copy), interval_);
        return true;
    }

    scheduler *sched_;
    std::chrono::milliseconds interval_;
};

int main() {
    scheduler sched;

    auto timer_2 = timer{&sched, 2000ms};
    auto timer_3 = timer{&sched, 3000ms};
    auto init_task = [&](int& ec){
        std::cout << "Initializing work" << std::endl;
        sched.schedule(timer_2, 0ms);
        sched.schedule(timer_3, 0ms);
        return true;
    };

    sched.schedule(init_task, 0ms);
    std::cout << "sched.run()" << std::endl;
    sched.run();
}
