#pragma once

#include "Perceptron.h"
#include "math/Matrix.h"

/**
 * Adaline neuron classifier using gradient decent method
 */
class AdalineGD : public Classifier
{
public:
    /**
     * default constructor
     * @param _eta learning rate
     * @param iter max number iterations
     */
    explicit AdalineGD(double _eta = 0.01, int iter = 10)
        : Classifier(_eta, iter) { }

    /**
     * fit method to train the classifier using gradient decent method
     * @param X input
     * @param y target output
     */
    void fit(const Matrix<double>& X, const Matrix<double>& y) override {
        initialize_weights(X.columns());
        costs = Matrix<double>(0, n_iter, 1);
        for(int iter = 0; iter < n_iter; iter++) {
            auto output = netInput(X);
            auto errors = y - output;

            auto delta = (X.Transpose() * errors) * eta;
            for(size_t i = 0; i < weights.rows(); i++) {
                for(size_t j = 0; j < weights.columns(); j++) {
                    if(i == 0) weights(i, j) += eta * errors.sumElements();
                    else
                        weights(i, j) += delta(i - 1, j);
                }
            }
            auto cost      = costFunction(errors);
            costs(iter, 0) = cost;
        }
    }

    /**
     * calculates
     * $$ X \cdot w + b
     * @param X input values
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
        B = Matrix<double>(B(0, 0), X.rows(), 1);
        return (X * A) + B;
    }

    /**
     * activates given input
     * @param X input
     * @return activated input
     */
    virtual Matrix<double> activation(const Matrix<double>& X) { return netInput(X); }

    /**
     * predicts class of given input
     * @param X input values
     * @return prediction
     */
    virtual Matrix<double> predict(const Matrix<double>& X) {
        std::function<bool(double)> condition = [](double x) { return bool(x >= 0.0); };
        return where(condition, activation(X), { { 1 } }, { { -1 } });
    }

    /**
     * calculates cost
     *
     * $$\frac{\sum X^2}{2}
     * @param X input values
     * @return squared cost
     */
    double costFunction(const Matrix<double>& X) { return HadamardMulti<double>(X, X).sumElements() / 2.0; }
};
/**
 * \example ds/TestAdalineGD.cpp
 * This is an example on how to use the AdalineGD class for binary classification.
 *
 */