/**
 * @file
 *
 * Regular forward substitution.
 * Calculates result given lower left triangular matrix
 * $$
 * b=L\cdot c \Rightarrow L^{-1}\cdot b = c
 * $$
 *
 * Requires:
 * \code
 * #include <math/numerics/lin_alg/forwardSub.h>
 * \endcode
 */

#pragma once
#include "../utils.h"
#include <iostream>
#include <vector>

/**
 * Forward-substitution
 * @param L Coefficient Matrix
 * @param b resulting vector
 * @return c
 */
Matrix<double> forwardSub(const Matrix<double>& L, const Matrix<double>& b) {
    size_t m = L.rows();
    size_t n = L.columns();
    size_t v = b.rows();
    if(v != m) {
        // Error, cannot compute
        std::cout << "Matrix vector dimension miss match, error!\n";
        return Matrix<double>();
    }
    if(m != n) {
        // Error, cannot compute
        std::cout << "Matrix not squared. Dimension miss match, error!\n";
        return Matrix<double>();
    }

    auto c = zerosV(v);

    c(0, 0) = b(0, 0);

    for(size_t j = 1; j < m; j++) {
        double s_k = 0.0f;
        for(size_t k = 0; k < j; k++) { s_k = s_k + (L(j, k) * c(k, 0)); }
        c(j, 0) = b(j, 0) - s_k;
    }

    return c;
}

/**
 * \example numerics/lin_alg/TestForwardSub.cpp
 * This is an example on how to use forwardSub.
 */