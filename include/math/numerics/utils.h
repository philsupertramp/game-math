/**
 * @file utils.h
 *
 *
 * Requires:
 * \code
 * #include <math/numerics/utils.h>
 * \endcode
 *
 * Numerical helper methods.
 *
 * This header includes numerical methods used to simplify computation of
 * numerical algorithms. It mostly contains generator like functions to generate
 * a specific kind of `Matrix`
 */

#pragma once
#include "../Matrix.h"
#include <vector>

#ifndef EPS
    //! accuracy of calculated results
    #define EPS 1e-8
#endif


/**
 * generates a vector with equidistant values, [start:(end-start)/num_elements:end]
 * @param start start value
 * @param end  end value
 * @param num_elements number of elements within the resulting vector
 * @return vector of equidistant values from start to end
 */
Matrix<double> linspace(double start, double end, unsigned long num_elements);

/**
 * Generates a matrix with each element = 0 for given size
 * @param rows number rows for resulting matrix
 * @param columns number columns for resulting matrix
 * @param elements number of elements per cell
 * @return rows x columns-Matrix with 0 as each value
 */
Matrix<double> zeros(size_t rows, size_t columns, size_t elements = 1);

/**
 * Generates a matrix with each element = 0 for given size
 * @param rows number of rows in resulting vetor
 * @return zero-vector of size rows
 */
Matrix<double> zerosV(size_t rows);

/**
 * Generates a matrix with each element = 1 for given size
 * @param rows number rows for resulting matrix
 * @param columns number columns for resulting matrix
 * @param elements number of elements per cell
 * @return rows x columns-Matrix with 1 as each value
 */
Matrix<double> ones(size_t rows, size_t columns = 1, size_t elements = 1);

/**
 * creates unit matrix of dimension rows, columns
 * @param rows
 * @param columns if 0, then columns = rows
 * @return
 */
Matrix<double> eye(size_t rows, size_t columns = 0);

/**
 * creates tri-diagonal-matrix with given values and dimensions
 * @param rows row dimension
 * @param columns column dimension
 * @param left left/lower diagonal element value
 * @param center diagonal element value
 * @param right right/upper diagonal element value
 * @return tri-diagonal-matrix
 */
Matrix<double> tridiag(size_t rows, size_t columns, double left, double center, double right);

/**
 * Calculates the p=2-Norm for a given matrix
 *
 * \f[
 *  \sqrt{\sum x_i^2}
 * \f]
 *
 * In 2D this is equal to the pythagorean theorem.
 *
 * @param in input matrix to calculate norm of
 * @return 2-norm of given matrix
 */
double norm(const Matrix<double>& in);


/**
 * Index-Getter for validated values
 *
 * Examples:
 *  validation function x<=0 returns indices of values <= 0
 *
 * @param validation validation function, this method validates your data
 * @param x vector to test
 * @return index vectors of values true to the validation function
 */
Matrix<size_t> nonzero(const std::function<bool(const double&)>& validation, const Matrix<double>& x);


template<typename T>
Matrix<T> diag(const Matrix<T>& in){
    bool isRows = in.rows() > in.columns();
    auto k = isRows ? in.rows() : in.columns();
    auto result = eye(k);
    for(size_t i = 0; i < k; ++i){
        result(i, i) = in(isRows ? i : 0, isRows ? 0 : i);
    }
    return result;
}

/**
 * \example numerics/TestUtils.cpp
 * This is an example on how to use the utils.h file.
 */