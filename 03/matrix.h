#pragma once

#include <ostream>
#include <vector>

class Row {
    private:
        std::vector<int> vec;

    public:
        Row(size_t);
        int operator[](const ssize_t) const;
        int& operator[](const ssize_t);

};

class Matrix {
    private:
        std::vector<Row> rowsVec;
        size_t rows;
        size_t cols;

    public:
        Matrix(size_t, size_t);
        size_t getRows() const;
        size_t getColumns() const;
        Matrix& operator*=(const int);
        bool operator==(const Matrix&) const;
        bool operator!=(const Matrix&) const;
        const Row& operator[](const ssize_t) const;
        Row& operator[](const ssize_t);
};