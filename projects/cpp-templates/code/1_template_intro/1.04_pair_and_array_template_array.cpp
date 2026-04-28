#include <iostream>
#include <cassert>

// --- Library Types ---

template <typename F, typename S>
struct pair {
    F first_;
    S second_;
};

// an array of N objects of type T
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

pair<int, double> get_max_and_average(const array<int, 8>& nums) {
    int curr_max = nums.at(0);
    double curr_sum = 0;
    for (std::size_t i = 0; i < 8; ++i) {
        curr_max = curr_max > nums.at(i) ? curr_max : nums.at(i);
        curr_sum += nums.at(i);
    }
    return {curr_max, curr_sum / 8};
}

// --- Application Code ---

int main() {
    array<int, 8> nums = {1, 2, 3, 20, 1, 2, 3, 4};
    pair<int, double> result = get_max_and_average(nums);
    std::cout << "Max: " << result.first_ << "\n";
    std::cout << "Avg: " << result.second_ << "\n";

    // Implicit instantiations of templates
    pair<int, int> a;
    pair<int, pair<int, int>> b;
    array<double, 42> c;
    array<char*, 200> d;
}

// Explicit instantiations of templates
template struct pair<char, char>;
template struct pair<int[10], char*>;
template struct array<pair<int, int>, 6771>;
    