#include <iostream>
#include <vector>
#include <fstream>

int main()
{
    std::vector<int> vec;

    if (vec.empty()) {
        std::cout << 1 << '\n';
    }
    std::ofstream out("2.txt", std::ios::binary);
    for (int i = 0; i < 20; ++i) {
        uint64_t tmp;
        std::cin >> tmp;
        out.write((char*) &tmp, sizeof(tmp));
    }
}