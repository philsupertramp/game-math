/**
 * @file
 *
 * This header file holds utility helper for ode solvers
 */

#pragma once
#include <functional>
#include <vector>

//! alias for ODE
using ODE    = std::function<Matrix<double>(double, Matrix<double>)>;

//! alias for Jacobian-Matrix of ODE
using ODEJac = std::function<Matrix<double>(double, Matrix<double>)>;

/**
 * Representation of ODE result
 */
struct ODEResult {
    //! Y-values
    Matrix<double> Y;
    //! T-values (mostly time)
    Matrix<double> T;
    //! number iterations per time step
    Matrix<int> Iterations;

    /**
     * Constructor without iterations
     * @param y
     * @param t
     */
    ODEResult(const Matrix<double>& y, const Matrix<double>& t)
        : Y(y)
        , T(t) { }

    /**
     * full element constructor
     * @param y
     * @param t
     * @param iter
     */
    ODEResult(const Matrix<double>& y, const Matrix<double>& t, const Matrix<int>& iter)
        : Y(y)
        , T(t)
        , Iterations(iter) { }
};
/**
 * Option struct for ODE solvers
 */
struct ODEOption {
    //! step width of t
    double h    = 0;
    //! tolerance for the algorithm to determine convergence
    double TOL  = 1e-7;
    //! max iterations for integrated newton steps
    int maxIter = 50;
    //! jacobian matrix of given function
    ODEJac Jac  = nullptr;
};