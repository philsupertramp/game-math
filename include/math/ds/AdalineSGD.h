#pragma once

#include "Classifier.h"
#include "math/Matrix.h"

class AdalineSGD : public Classifier
{
public:
    bool shuffle;
    int randomState;

private:
    Matrix<double> update_weights(const Matrix<double>& xi, const Matrix<double>& target) {
        auto output = netInput(xi);
        auto error  = target - output;
        for(size_t i = 0; i < weights.rows(); i++) {
            for(size_t j = 0; j < weights.columns(); j++) {
                if(i == 0) weights(i, j) += (error * eta)(i, j);
                else
                    weights(i, j) += ((xi.Transpose() * error) * eta)(i - 1, j);
            }
        }
        return (error * error) * 0.5;
    }

public:
    explicit AdalineSGD(double _eta = 0.01, int iter = 10, bool _shuffle = false, int _randomState = 0)
        : Classifier(_eta, iter)
        , shuffle(_shuffle)
        , randomState(_randomState) {
        if(randomState != 0) Random::SetSeed(randomState);
    }

    /**
     *
     * @param X: array-like with the shape: [n_samples, n_features]
     * @param y: array-like with shape: [n_samples, 1]
     * @return this
     */
    void fit(const Matrix<double>& X, const Matrix<double>& y) override {
        initialize_weights(X.columns());
        costs      = Matrix<int>(0, n_iter, 1);
        auto xCopy = X;
        auto yCopy = y;
        for(int iter = 0; iter < n_iter; iter++) {
            if(shuffle) {
                auto fooPair = shuffleData(X, y);
                xCopy        = fooPair.first;
                yCopy        = fooPair.second;
            }
            double costSum = 0;
            for(const auto& elem : zip(xCopy, yCopy)) {
                costSum += update_weights(elem.first, elem.second).sumElements();
            }

            costs(iter, 0) = costSum / yCopy.rows();
        }
    }


    void partial_fit(const Matrix<double>& X, const Matrix<double>& y) {
        if(!w_initialized) { initialize_weights(X.columns()); }
        if(y.rows() > 1) {
            for(const auto& elem : zip(X, y)) { update_weights(elem.first, elem.second); }
        } else {
            update_weights(X, y);
        }
    }

    std::pair<Matrix<double>, Matrix<double>>
    shuffleData([[maybe_unused]] const Matrix<double>& X, [[maybe_unused]] const Matrix<double>& y) {
        return { X, y };
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