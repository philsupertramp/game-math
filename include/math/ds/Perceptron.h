#pragma once

#include "Classifier.h"
#include "MatrixDS.h"

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
    explicit Perceptron(double _eta = 0.01, int iter = 10)
        : Classifier(_eta, iter) { }

    /**
     *
     * @param X: array-like with the shape: [n_samples, n_features]
     * @param y: array-like with shape: [n_samples, 1]
     * @return this
     */
    void fit(const MatrixDS<double>& X, const MatrixDS<double>& y) override {
        initialize_weights(X.columns());
        costs = MatrixDS<int>(0, n_iter, 1);
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
                        if(i == 0) weights[i][j] += update[i][j];
                        else
                            weights[i][j] += delta[i - 1][j];
                    }
                }
                _errors += costFunction(update);
            }
            costs[iter][0] = _errors;
        }
    }

    MatrixDS<double> netInput(const MatrixDS<double>& X) override {
        MatrixDS<double> A, B;
        A.Resize(weights.rows() - 1, weights.columns());
        B.Resize(1, weights.columns());
        for(size_t i = 0; i < weights.rows(); i++) {
            for(size_t j = 0; j < weights.columns(); j++) {
                if(i == 0) {
                    B[i][j] = weights[i][j];
                } else {
                    A[i - 1][j] = weights[i][j];
                }
            }
        }
        return (X * A) + B;
    }
    virtual MatrixDS<double> activation(const MatrixDS<double>& X) override { return netInput(X); }

    virtual MatrixDS<double> predict(const MatrixDS<double>& X) override {
        std::function<bool(double)> condition = [](double x) { return bool(x >= 0.0); };
        return where(condition, activation(X), { { 1 } }, { { -1 } });
    }

    virtual double costFunction(const MatrixDS<double>& X) override { return (double)(X[0][0] != 0.0); }
};