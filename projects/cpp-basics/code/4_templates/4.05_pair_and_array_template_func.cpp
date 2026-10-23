#include <iostream>
#include <cassert>

// --- Library Types ---

template <typename F, typename S>
struct Pair {
    F mFirst;
    S mSecond;
};

template <typename T, std::size_t N>
// requries: 
// - T must be default constructible
// - N > 0
struct Array {
    T mData[N];

    // set array element to a value by index
    void set(std::size_t index, T val) {
        assert(index < N);
        mData[index] = val;
    }

    // get const reference to array element by index
    const T& at(std::size_t index) const {
        assert(index < N);
        return mData[index];
    }
};

// --- Library Code ---

template <typename T, std::size_t N>
// requires: T - convertible to double
Pair<T, double> getMaxAndAverage(const Array<T, N>& nums) {
    auto currMax = nums.at(0);
    auto currSum = 0.0;
    for (std::size_t i = 0; i < N; ++i) {
        currMax = currMax > nums.at(i) ? currMax : nums.at(i);
        currSum += nums.at(i);
    }
    return {currMax, currSum / N};
}

// --- Application Code ---

int main() {
    // <T, N> = <int, 8>
    auto nums = Array<int, 8>{1, 2, 3, 20, 1, 2, 3, 4};
    auto result = getMaxAndAverage(nums); // getMaxAndAverage<int, 8>(nums);
    std::cout << "Max: " << result.mFirst << "\n";
    std::cout << "Avg: " << result.mSecond << "\n";

    // <T, N> = <double, 5>
    auto nums2 = Array<double, 5>{1, 5, 4, 3, 2};
    auto result2 = getMaxAndAverage(nums2); // getMaxAndAverage<double, 5>(nums);
    std::cout << "Max: " << result.mFirst << "\n";
    std::cout << "Avg: " << result.mSecond << "\n";
}

// You can also explicitly instantiate template functions
template Pair<char, double> getMaxAndAverage<char, 20>(const Array<char, 20>& a);
    