#include "numbers.dat"
#include <iostream>
#include <functional>
#include <cmath>
#include <vector>
#include <sstream>

enum {
    MIN = 0,
    MAX = 100000
};

std::vector<bool> sieve(const int min = MIN, const int max = MAX) 
{
    std::vector<bool> vec(max - min + 1, true);
    vec[0] = false;
    vec[1] = false;
    for (int i = 2; i <= max - min; ++i) {
        if (vec[i]) {
            if (i *1ll * i <= max - min) {
                for (int j = i * i; j <= max - min; j += i) {
                    vec[j] = false;
                }
            }
        }
    }
    return vec;
}

//binary search
int findIndex(const int *arr, const int number, int right, std::function<bool (int, int)> func) 
{
    int left = -1;
    while (left < right - 1) {
        int middle = (left + right) / 2;
        if (func(arr[middle], number)) {
            left = middle;
        } else {
            right = middle;
        }
    }
    if (arr[right] != number && arr[right - 1] != number) {
        return -1;
    }
    return right;
}

int primesCount(const int* data, const std::vector<bool> &primes, const int leftIdx, const int rightIdx) 
{
    int count = 0;
    for (int i = leftIdx; i < rightIdx; ++i) {
        if (primes[data[i]]) {
            ++count;
        }
    }
    return count;
}

int main(int argc, char const* argv[])
{
    if (!(argc % 2) || argc == 1) {
        return -1;
    }
    auto primes = sieve();
    std::stringstream ss;
    for (int i = 1; i < argc; ++i) {
        int left;
        int right;
        try {
            left = std::stoi(argv[i++]);
            right = std::stoi(argv[i]);
        } catch (...) {
            return -1;
        }
        if (left < MIN || right > MAX || left < 0 || right < 0) {
            return -1;
        }
        int leftIdx = findIndex(Data, left, Size, [](int x, int y) {
                                                        return x < y;
                                                    });
        int rightIdx = findIndex(Data, right, Size, [](int x, int y) {
                                                        return x <= y;
                                                    });
        if (leftIdx < 0 || rightIdx < 0 || left > right) {
            ss << 0 << std::endl;
            continue;
        }
        ss << primesCount(Data, primes, leftIdx, rightIdx) << std::endl;
    }
    std::cout << ss.str();
}