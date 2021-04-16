#pragma once

#include "Perceptron.h"
#include "math/Matrix.h"

/**
 * Adaline neuron classifier using gradient decent method
 */
class AdalineGD : public Classifier
{
public:
    explicit AdalineGD(double _eta = 0.01, int iter = 10)
        : Classifier(_eta, iter) { }

    /**
     * fit method to train the classifier using gradient decent method
     */
    void fit(const Matrix<double>& X, const Matrix<double>& y) override {
        initialize_weights(X.columns());
        costs = Matrix<int>(0, n_iter, 1);
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

    virtual Matrix<double> activation(const Matrix<double>& X) override { return netInput(X); }

    virtual Matrix<double> predict(const Matrix<double>& X) override {
        std::function<bool(double)> condition = [](double x) { return bool(x >= 0.0); };
        return where(condition, activation(X), { { 1 } }, { { -1 } });
    }

    double costFunction(const Matrix<double>& X) override { return HadamardMulti<double>(X, X).sumElements() / 2.0; }
};