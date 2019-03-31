#include "matrix.h"
#include <stdexcept>

// Row proxy-class
Row::Row(size_t x = 0) 
{
    vec = std::vector<int>(x);
}

int Row::operator[](ssize_t i) const 
{
    if (i < 0 || i >= vec.size()) {
        throw std::out_of_range("index is out range");
    }
    
    return vec[i];
}

int& Row::operator[](ssize_t i) 
{
    if (i < 0 || i >= vec.size()) {
        throw std::out_of_range("index is out range");
    }

    return vec[i];
}

// Matrix class
Matrix::Matrix(size_t rows = 0, size_t columns = 0) : rows(rows), cols(columns), rowsVec(std::vector<Row>(rows, columns))
{

}

size_t Matrix::getRows() const
{
    return this->rows;
}

size_t Matrix::getColumns() const
{
    return this->cols;
}

const Row& Matrix::operator[](const ssize_t i) const
{
    if (i < 0 || i >= rowsVec.size()) {
        throw std::out_of_range("index is out range");
    }

    return rowsVec[i];
}

Row& Matrix::operator[](const ssize_t i) 
{
    if (i < 0 || i >= rowsVec.size()) {
        throw std::out_of_range("index is out range");
    }

    return rowsVec[i];
};

Matrix& Matrix::operator*=(const int mult)
{
    for (auto &row : rowsVec) {
        for (size_t i = 0; i < this->getColumns(); ++i) {
            row[i] *= mult;
        }
    }

    return *this;
}

bool Matrix::operator==(const Matrix &other) const 
{
    if (this == &other) {
        return true;
    }

    if (this->getColumns() != other.getColumns() || this->getRows() != other.getRows()) {
        return false;
    }

    for (size_t i = 0; i < this->getRows(); ++i) {
        for (size_t j = 0; j < this->getColumns(); ++j) {
            if ((*this)[i][j] != other[i][j]) {
                return false;
            }
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix &other) const
{
    return !(*this == other);
}