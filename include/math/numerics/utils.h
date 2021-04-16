#pragma once
#include "../Matrix.h"
#include <vector>

#ifndef EPS
    #define EPS 1e-8
#endif


Matrix<double> linspace(double start, double end, unsigned long num_elements);

Matrix<double> zeros(size_t rows, size_t columns);

Matrix<double> ones(size_t rows, size_t columns = 1);

/**
 * creates unit matrix of dimension rows, columns
 * @param rows
 * @param columns if 0, then columns = rows
 * @return
 */
Matrix<double> eye(size_t rows, size_t columns = 0);

Matrix<double> zerosV(size_t rows);

double norm(const Matrix<double>& in);
