#include <iostream>
#include "calc.h"

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        std::cerr << "error" << std::endl;
        return 1;
    }
    Calc calc(argv[1]);
    int64_t result;
    try {
        result = calc.compute();
    } catch (const std::logic_error &ex) {
        std::cerr << "error" << std::endl;
        return 1;
    }
    std::cout << result << std::endl;
    return 0;
}