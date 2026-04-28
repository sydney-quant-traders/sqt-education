#include <iostream>
#include <cassert>

// --- Library Types ---

template <typename F, typename S>
struct pair {
    F first_;
    S second_;
};

template <typename T, std::size_t N>
// requries: 
// - T must be default constructible
// - N > 0
struct array {
    T data_[N];

    // set array element to a value by index
    void set(std::size_t index, T val) {
        assert(index < N);
        data_[index] = val;
    }

    // get const reference to array element by index
    const T& at(std::size_t index) const {
        assert(index < N);
        return data_[index];
    }
};

// --- Library Code ---

template <typename T, std::size_t N>
// requires: T - convertible to double
pair<T, double> get_max_and_average(const array<T, N>& nums) {
    T curr_max = nums.at(0);
    double curr_sum = 0;
    for (std::size_t i = 0; i < N; ++i) {
        curr_max = curr_max > nums.at(i) ? curr_max : nums.at(i);
        curr_sum += nums.at(i);
    }
    return {curr_max, curr_sum / N};
}

// --- Application Code ---

int main() {
    // <T, N> = <int, 8>
    array<int, 8> nums = {1, 2, 3, 20, 1, 2, 3, 4};
    pair<int, double> result = get_max_and_average(nums); // get_max_and_average<int, 8>(nums);
    std::cout << "Max: " << result.first_ << "\n";
    std::cout << "Avg: " << result.second_ << "\n";

    // <T, N> = <double, 5>
    array<double, 5> nums2 = {1, 5, 4, 3, 2};
    pair<double, double> result2 = get_max_and_average(nums2); // get_max_and_average<double, 5>(nums);
    std::cout << "Max: " << result.first_ << "\n";
    std::cout << "Avg: " << result.second_ << "\n";
}

// You can also explicitly instantiate template functions
template pair<char, double> get_max_and_average<char, 20>(const array<char, 20>& a);
    