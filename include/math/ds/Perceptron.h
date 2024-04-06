#pragma once

#include "Classifier.h"
#include "../Matrix.h"

/**
 * Represents Perceptron definition by Frank Rosenblatt
 *
 * [Wikipedia](https://en.wikipedia.org/wiki/Perceptron)
 *
 * Essentially it can be used to build binary classification
 * One might for example want to replace some very complex binary operation
 * which is a combination of several broad operations like OR|AND|XOR|NOT ...
 *
 * In order to use the perceptron classifier one needs access to the full set
 * of possible transformations.
 *
 * THIS IS NOT A PREDICTOR
 *
 * Mainly implemented for academic use
 */
class Perceptron : public Classifier
{
public:
    /**
     * default constructor
     * @param _eta
     * @param iter
     */
    explicit Perceptron(double _eta = 0.01, int iter = 10)
        : Classifier(_eta, iter) { }

    /**
     *
     * @param X: array-like with the shape: [n_samples, n_features]
     * @param y: array-like with shape: [n_samples, 1]
     * @return this
     */
    void fit(const Matrix<double>& X, const Matrix<double>& y) override {
        initialize_weights(X.columns());
        costs = Matrix<double>(0, n_iter, 1);
        for(int iter = 0; iter < n_iter; iter++) {
            int _errors   = 0;
            auto iterable = zip(X, y);
            for(const auto& elem : iterable) {
                auto xi     = elem.first;
                auto target = elem.second;

                auto update = (target - predict(xi));
                auto delta  = (update * xi).Transpose() * eta;

                for(size_t i = 0; i < weights.rows(); i++) {
                    for(size_t j = 0; j < weights.columns(); j++) {
                        if(i == 0) weights(i, j) += update(i, j);
                        else
                            weights(i, j) += delta(i - 1, j);
                    }
                }
                _errors += costFunction(update);
            }
            costs(iter, 0) = _errors;
        }
    }

    /**
     * calculates net input
     * \f[
     *      X * weights + b
     * \f]
     * @param X
     * @return
     */
    Matrix<double> netInput(const Matrix<double>& X) override {
        Matrix<double> A, B;
        A.Resize(weights.rows() - 1, weights.columns());
        B.Resize(1, weights.columns());
        for(size_t i = 0; i < weights.rows(); i++) {
            for(size_t j = 0; j < weights.columns(); j++) {
                if(i == 0) {
                    B(i, j) = weights(i, j);
                } else {
                    A(i - 1, j) = weights(i, j);
                }
            }
        }
        return (X * A) + B;
    }

    /**
     * activate given input
     * @param X
     * @return
     */
    virtual Matrix<double> activation(const Matrix<double>& X) { return netInput(X); }

    /**
     * predict class of given input
     * @param X
     * @return
     */
    virtual Matrix<double> predict(const Matrix<double>& X) {
        std::function<bool(double)> condition = [](double x) { return bool(x >= 0.0); };
        return where(condition, activation(X), { { 1 } }, { { -1 } });
    }

    /**
     * cost function override, don't use!
     * @param X
     * @return
     */
    virtual double costFunction(const Matrix<double>& X) { return (double)(X(0, 0) != 0.0); }
};

/**
 * \example ds/TestPerceptron.cpp
 * This is an example on how to use the Perceptron class for binary classification.
 *
 */
