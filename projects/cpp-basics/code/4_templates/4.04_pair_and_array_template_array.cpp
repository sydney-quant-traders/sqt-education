#include <iostream>
#include <cassert>

// --- Library Types ---

template <typename F, typename S>
struct Pair {
    F mFirst;
    S mSecond;
};

// an array of N objects of type T
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

Pair<int, double> getMaxAndAverage(const Array<int, 8>& nums) {
    auto currMax = nums.at(0);
    auto currSum = 0.0;
    for (std::size_t i = 0; i < 8; ++i) {
        currMax = currMax > nums.at(i) ? currMax : nums.at(i);
        currSum += nums.at(i);
    }
    return {currMax, currSum / 8};
}

// --- Application Code ---

int main() {
    auto nums = Array<int, 8>{1, 2, 3, 20, 1, 2, 3, 4};
    auto result = getMaxAndAverage(nums);
    std::cout << "Max: " << result.mFirst << "\n";
    std::cout << "Avg: " << result.mSecond << "\n";

    // Implicit instantiations of templates
    auto a = Pair<int, int>{};
    auto b = Pair<int, Pair<int, int>>{};
    auto c = Array<double, 42>{};
    auto d = Array<char*, 200>{};
}

// Explicit instantiations of templates
template struct Pair<char, char>;
template struct Pair<int[10], char*>;
template struct Array<Pair<int, int>, 6771>;
    