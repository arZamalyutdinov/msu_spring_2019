#pragma once
#include <string>
#include <exception>

class Calc {
    public:
        Calc(const std::string &);
        int64_t compute();

    protected:
        int64_t term(std::string::iterator &);
        int64_t prim(std::string::iterator &);
        int64_t expression(std::string::iterator &);
    
    private:
        std::string expr;
};