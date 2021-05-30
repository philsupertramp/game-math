/**
 * @file Probability.h
 *
 * Contains methods of the field of probability calculation.
 */
#pragma once

#include "../Matrix.h"

/**
 * Linear Regression Model
 */
struct LinearModel {
    //! $$\beta_0$$
    double beta_0;
    //! $$\beta_1$$
    double beta_1;
    //! $$\hat{y}$$ approximated y value for given samples
    Matrix<double> y_estimate;
    //! $$\epsilon_i$$ Residuals of the underlying squared error computations
    Matrix<double> residuals;
};

/**
 * enum to chose a norm to compute
 */
enum P_NORM {
    //! $$||x||_\infty$$
    Inf,
    //! $$||x||_1$$
    One,
    //! $$||x||_2$$
    Eukl
};

/** regular helpers **/
/**
 * Method to calculate exponent of given value
 * @param x value to calculate exponent of
 * @return exponent
 */
double getExponent(double x);
/**
 * Rounds double value to given precision, cuts off other elements
 * @param x value to round
 * @param precision precision in resulting value
 * @return rounded value
 */
double round(double x, int precision);
/**
 * Absolute value $$|x|$$
 * @param x a value
 * @return $$|x|$$
 */
double abs(double x);
/**
 * calculates $$x^{exponent}$$
 * @param x a value
 * @param exponent desired exponent
 * @return $$x^{exponent}$$
 */
double pow(double x, int exponent);
/**
 * Calculates the Norm of a given vector
 * @param vec vector to calculate the norm of
 * @param pNorm desired norm to compute
 * @return computed norm value
 */
double norm(const Matrix<double>& vec, P_NORM pNorm);
/**
 * Computes covariance of two given vectors
 * $$\text{cov}(x, y)$$
 * @param x a vector
 * @param y another vector
 * @return resulting covariance
 */
double cov(const Matrix<double>& x, const Matrix<double>& y);
/**
 * Computes variance of given vector
 * $$\text{var}(x)$$
 * @param x a vector
 * @return $$\text{var}(x)$$
 */
double var(const Matrix<double>& x);


/**
 * Computes linear (regression) model for given random sample
 * $$
 * (Y_i; X_i)
 * $$
 * $$
 * Y_i = \beta_0 + \beta_1\phi_1(x_i) + \epsilon_i
 * $$
 * @param x $$X_i$$
 * @param y $$Y_i$$
 * @return constructed linear model
 */
LinearModel lm(const Matrix<double>& x, const Matrix<double>& y);
/**
 *$$\frac{\text{var}(y)}{\text{var}(\hat{y})}$$
 * @param y $$y$$
 * @param yHat $$\hat{y}$$
 * @return
 */
double coefficientOfDetermination(const Matrix<double>& y, const Matrix<double>& yHat);
/**
 *
 * @return
 */
double likelihood(const Matrix<double>&);

/**
 * \example statistics/TestProbability.cpp
 * This is an example on how to use the Porbability.h file.
 */