#include "utils.h"
#include <cmath>

Matrix<double> linspace(double start, double end, unsigned long num_elements) {
    Matrix<double> result(0, 1, num_elements, 1);
    double h = (end - start) / (double)(num_elements - 1);
    for(size_t i = 0; i < num_elements; i++) { result(0, i) = start + double(i) * h; }
    return result;
}

Matrix<double> zeros(size_t rows, size_t columns) { return Matrix<double>(0, rows, columns, 1); }
Matrix<double> ones(size_t rows, size_t columns) { return Matrix<double>(1, rows, columns); }

Matrix<double> eye(size_t rows, size_t columns) {
    auto realCols = columns;
    if(columns == 0) realCols = rows;
    auto out = zeros(rows, realCols);
    for(size_t i = 0; i < rows && i < realCols; ++i) { out(i, i) = 1.0; }
    return out;
}
double norm(const Matrix<double>& in) {
    double out = 0;
    for(unsigned long i = 0; i < in.rows(); ++i) {
        for(unsigned long j = 0; j < in.columns(); ++j) { out += in(i, j) * in(i, j); }
    }
    return sqrt(out);
}
Matrix<double> zerosV(size_t rows) { return Matrix<double>(0.0f, rows, 1); }
