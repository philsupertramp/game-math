#pragma once

#include "Classifier.h"
#include "MatrixDS.h"

class AdalineSGD : public Classifier
{
public:
    bool shuffle;
    int randomState;

private:
    MatrixDS<double> update_weights(const MatrixDS<double>& xi, const MatrixDS<double>& target) {
        auto output = netInput(xi);
        auto error  = target - output;
        for(size_t i = 0; i < weights.rows(); i++) {
            for(size_t j = 0; j < weights.columns(); j++) {
                if(i == 0) weights[i][j] += (error * eta)[i][j];
                else
                    weights[i][j] += ((xi.Transpose() * error) * eta)[i - 1][j];
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
    void fit(const MatrixDS<double>& X, const MatrixDS<double>& y) override {
        initialize_weights(X.columns());
        costs      = MatrixDS<int>(0, n_iter, 1);
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

            costs[iter][0] = costSum / yCopy.rows();
        }
    }


    void partial_fit(const MatrixDS<double>& X, const MatrixDS<double>& y) {
        if(!w_initialized) { initialize_weights(X.columns()); }
        if(y.rows() > 1) {
            for(const auto& elem : zip(X, y)) { update_weights(elem.first, elem.second); }
        } else {
            update_weights(X, y);
        }
    }

    std::pair<MatrixDS<double>, MatrixDS<double>>
    shuffleData([[maybe_unused]] const MatrixDS<double>& X, [[maybe_unused]] const MatrixDS<double>& y) {
        return { X, y };
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
        B = MatrixDS<double>(B[0][0], X.rows(), 1);
        return (X * A) + B;
    }

    /**
     * Do not use
     * @param X
     * @return
     */
    double costFunction([[maybe_unused]] const MatrixDS<double>& X) override { return 0; }

    virtual MatrixDS<double> activation(const MatrixDS<double>& X) override { return netInput(X); }

    virtual MatrixDS<double> predict(const MatrixDS<double>& X) override {
        std::function<bool(double)> condition = [](double x) { return bool(x >= 0.0); };
        return where(condition, activation(X), { { 1 } }, { { -1 } });
    }
};