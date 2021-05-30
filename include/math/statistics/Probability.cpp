#include "Probability.h"


/** regular helpers **/
double round(double x, int precision) {
    int prec      = pow(10, precision);
    int lastDigit = abs((int)(x * prec * 10) % 10);
    return ((int)(x * prec) + (lastDigit > 0 && lastDigit >= 5 ? 1 : 0) * (x > 0 ? 1 : -1)) / (double)prec;
}
double abs(double x) {
    if(x < 0.) return x * (-1.);
    return x;
}
double pow(double x, int p) {
    if(p == 0) return 1;
    if(p < 0) return 1 / pow(x, -1 * p);
    double result = x;
    for(int i = 0; i < p - 1; i++) result = result * x;
    return result;
}
double norm(const Matrix<double>& x, P_NORM pNorm) {
    // Vector-norm!
    assert(x.columns() == 1 || x.rows() == 1);
    bool useRows = x.rows() > x.columns();
    size_t size  = useRows ? x.rows() : x.columns();
    switch(pNorm) {
        case P_NORM::Inf:
            {
                double maximum = x(0, 0);
                for(int i = 1; i < size; i++) {
                    if(abs(x(useRows ? i : 0, useRows ? 0 : i)) > maximum) {
                        maximum = abs(x(useRows ? i : 0, useRows ? 0 : i));
                    }
                }
                return maximum;
            }
        case P_NORM::One:
            {
                double sum = 0.0;
                for(int i = 0; i < size; i++) { sum += abs(x(useRows ? i : 0, useRows ? 0 : i)); }
                return sum;
            }
        case P_NORM::Eukl:
        default:
            {
                double sum = 0.0;
                for(int i = 0; i < size; i++) { sum += pow(x(useRows ? i : 0, useRows ? 0 : i), 2.0); }
                return sqrt(sum);
            }
    }
}
double cov(const Matrix<double>& x, const Matrix<double>& y) {
    double sum = 0.;
    x.assertSize(y);
    bool useRows = x.rows() > x.columns();
    size_t size  = useRows ? x.rows() : x.columns();
    for(int i = 0; i < size; i++) {
        sum = sum + x(useRows ? i : 0, useRows ? 0 : i) * y(useRows ? i : 0, useRows ? 0 : i);
    }
    double mean_val = mean(x) * mean(y);
    return (1. / double(size - 1) * sum) - (double(size) / double(size - 1) * mean_val);
}
double var(const Matrix<double>& x) {
    double x_mean = mean(x);
    double sum    = 0.;
    bool useRows  = x.rows() > x.columns();
    size_t size   = useRows ? x.rows() : x.columns();
    for(int i = 0; i < size; i++) { sum = sum + pow(x(useRows ? i : 0, useRows ? 0 : i), 2); }
    return abs((1. / double(size - 1) * sum) - (double(size) / double(size - 1) * pow(x_mean, 2)));
}

/** probability computation **/
LinearModel lm(const Matrix<double>& x, const Matrix<double>& y) {
    x.assertSize(y);
    double covar    = cov(x, y);
    double variance = var(x);

    double beta_1 = covar / variance;
    double beta_0 = mean(y) - beta_1 * mean(x);
    bool useRows  = x.rows() > x.columns();
    size_t size   = useRows ? x.rows() : x.columns();

    Matrix<double> estimate, residuals;
    estimate.Resize(size, 1);
    residuals.Resize(size, 1);
    for(int i = 0; i < size; i++) {
        estimate(i, 0)  = beta_0 + beta_1 * x(useRows ? i : 0, useRows ? 0 : i);
        residuals(i, 0) = y(useRows ? i : 0, useRows ? 0 : i) - estimate(i, 0);
    }
    return { beta_1, beta_0, estimate, residuals };
}
double coefficientOfDetermination(const Matrix<double>& y, const Matrix<double>& yHat) {
    double varY    = var(y);
    double varYHat = var(yHat);
    return varY / varYHat;
}
double getExponent(double x) {
    double exponent = 1;
    while(abs(x / pow(10, exponent)) > 1) { exponent = exponent + 1; }
    return exponent;
}
double likelihood(const Matrix<double>& vec) {
    for(int i = 0; i < vec.rows(); ++i) {
        //vec->_e[i];
    }
}
