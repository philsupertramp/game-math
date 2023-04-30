#include "../../include/math/numerics/utils.h"
#include "../../include/math/Matrix.h"
#include <cmath>

Matrix<double> linspace(double start, double end, unsigned long num_elements) {
    Matrix<double> result(0, 1, num_elements, 1);
    double h = (end - start) / (double)(num_elements - 1);
    for(size_t i = 0; i < num_elements; i++) { result(0, i) = start + double(i) * h; }
    return result;
}

Matrix<double> zeros(size_t rows, size_t columns, size_t elements) {
    return Matrix<double>(0, rows, columns, elements);
}

Matrix<double> ones(size_t rows, size_t columns, size_t elements) { return Matrix<double>(1, rows, columns, elements); }

Matrix<double> eye(size_t rows, size_t columns) {
    auto realCols = columns;
    if(columns == 0) realCols = rows;
    auto out = zeros(rows, realCols);
    for(size_t i = 0; i < rows && i < realCols; ++i) { out(i, i) = 1.0; }
    return out;
}
Matrix<double> tridiag(size_t rows, size_t cols, double lower, double center, double upper) {
    auto out = zeros(rows, cols);
    for(size_t row = 0; row < rows; ++row) {
        if(row > 0) out(row, row - 1) = lower;

        out(row, row) = center;

        if(row < rows - 1) out(row, row + 1) = upper;
    }
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


Matrix<size_t> nonzero(const std::function<bool(const double&)>& validation, const Matrix<double>& x) {
    // only allowed for vectors!
    assert(x.rows() == 1 || x.columns() == 1);
    auto xi = x;
    if(x.rows() < x.columns()) xi = x.Transpose();
    std::vector<size_t> vals;
    for(size_t i = 0; i < xi.rows(); ++i) {
        if(validation(xi(i, 0))) vals.push_back(i);
    }
    return Matrix<size_t>(&vals[0], vals.size());
}
