#pragma once

#include "Classifier.h"
#include "SGD.h"
#include "math/Matrix.h"

/**
 * Adaline linear neuron implementation using statistic gradient decent
 * for training of a vector of weights and a single bias.
 *
 * weights: shape [N+1 x 1]
 * bias: shape [1, 1], first element of weights
 *
 * Note: only allows binary classification and uses a single layer of weights
 *       see the description here [1] I found it very easy to understand and
 *       implement.
 *
 *       - [1] http://rasbt.github.io/mlxtend/user_guide/classifier/Adaline/
 */
class AdalineSGD : public Classifier
{
public:
    bool shuffle;
    int randomState;
    SGD sgd;

public:
    explicit AdalineSGD(double _eta = 0.01, int iter = 10, bool _shuffle = false, int _randomState = 0)
        : Classifier(_eta, iter)
        , shuffle(_shuffle)
        , randomState(_randomState) {
        if(randomState != 0) Random::SetSeed(randomState);
        sgd = SGD(_eta, iter, _shuffle);
    }

    /**
     *
     * @param X: array-like with the shape: [n_samples, n_features]
     * @param y: array-like with shape: [n_samples, 1]
     * @return this
     */
    void fit(const Matrix<double>& X, const Matrix<double>& y) override {
        initialize_weights(X.columns());
        sgd.fit(X, y, weights);
    }


    void partial_fit(const Matrix<double>& X, const Matrix<double>& y) {
        if(!w_initialized) { initialize_weights(X.columns()); }
        sgd.partial_fit(X, y, weights);
    }

    Matrix<double> netInput(const Matrix<double>& X) override { return sgd.netInput(X, weights); }

    /**
     * Do not use
     * @param X
     * @return
     */
    double costFunction([[maybe_unused]] const Matrix<double>& X) override { return 0; }

    virtual Matrix<double> activation(const Matrix<double>& X) override { return netInput(X); }

    virtual Matrix<double> predict(const Matrix<double>& X) override {
        std::function<bool(double)> condition = [](double x) { return bool(x >= 0.0); };
        return where(condition, activation(X), { { 1 } }, { { -1 } });
    }
};