#include "calc.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

Calc::Calc(const std::string &str) : expr(str) {
    expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());
}

int64_t Calc::compute() {
    auto curr = expr.begin();
    return expression(curr);
}

int64_t Calc::expression(std::string::iterator &curr) {
    int64_t left = term(curr);
    while (true) {
        if (*curr == '+') {
            left += term(++curr);
        } else if (*curr == '-') {
            left -= term(++curr);
        } else {
            break;
        }
    }
    return left;
}

int64_t Calc::term(std::string::iterator &curr) {
    int64_t left = prim(curr);
    while (true) {
        if (*curr == '*') {
            left *= prim(++curr);
        } else if (*curr == '/') {
            int64_t right = prim(++curr);
            if (right == 0) {
                throw std::logic_error("Division by zero");
            }
            left /= right;
        } else {
            break;
        }
    }
    return left;
}

int64_t Calc::prim(std::string::iterator &curr) {
    if (*curr >= '0' && *curr <= '9') {
        int64_t val = 0;
        if (!sscanf(expr.c_str() + (curr - expr.begin()), "%d", &val)) {
            throw std::logic_error("Wrong input");
        }
        while (*curr >= '0' && *curr <= '9') {
            ++curr;
        }
        return val;
    }
    if (*curr == '-') {
        return -prim(++curr);
    }
    throw std::logic_error("Wrong input");
}