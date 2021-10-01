#pragma once
#include "../../Matrix.h"
#include "../../utils.h"


/**
 * Calculates QR decomposition of given matrix
 * so in = Q * R
 * @param in matrix to calculate with
 * @return { Q, R } decomposition
 */
std::pair<Matrix<double>, Matrix<double>> qr(const Matrix<double>& in) {
    auto R = zeros(in.rows(), in.rows());
    auto Q = eye(in.rows(), in.rows());
    auto A = in;
    for(size_t i = 0; i < (in.rows() < in.columns() ? in.rows() : in.columns()); i++) {
        // perform givens rotation
        auto v      = A.GetSlice(0, A.rows() - 1, 0, 0);
        auto s      = sign(v(0, 0));
        v(0, 0)     = v(0, 0) + s * norm(v);
        auto qSlice = Q.GetSlice(0, Q.rows() - 1, i, Q.columns() - 1).Transpose();
        auto vTv    = (v.Transpose() * v)(0, 0);
        auto b      = (2.0f) / (vTv)*v;
        auto slice  = (qSlice - (b * (v.Transpose() * qSlice))).Transpose();
        Q.SetSlice(0, Q.rows() - 1, i, Q.columns() - 1, slice);

        // calculate Q, R
        A = A - b * (v.Transpose() * A);
        R.SetSlice(i, i, i, R.columns() - 1, A.GetSlice(0, 0, 0, A.columns() - 1));

        // remove calculated bits and reduce dimension
        A = A.GetSlice(1, A.rows() - 1, 1, A.columns() - 1);
    }
    return { Q, R };
}

/**
 * \example numerics/lin_alg/TestQR.cpp
 * This is an example on how to use the qr decomposition.
 */
