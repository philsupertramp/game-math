#pragma once

#include "../Matrix.h"
#include "Classifier.h"
#include "utils.h"

class LogRegSGD : public Classifier
{
public:
    bool shuffle = false;
    int randomState;
    SGD sgd;
    bool sgdInit = false;

private:
    void initialize_weights(size_t m) {
        weights       = Matrix<double>(0, m, 1);
        w_initialized = true;
    }


    /**
     * logistical regression:
     */
    double update_weights(const Matrix<double>& xi, const Matrix<double>& target) {
        auto output      = netInput(xi);
        auto error       = target - output;
        auto class1_cost = (target * -1.0) * Log(output);
        auto class2_cost = (Matrix<double>(1, output.rows(), output.columns()) - target)
                           * Log(Matrix<double>(1, output.rows(), output.columns()) - output);
        auto cost = class1_cost - class2_cost;

        auto gradient = (xi.Transpose() * error) * eta;
        gradient      = gradient * (1.0 / xi.rows());

        this->weights = this->weights - gradient;

        return cost.sumElements() / xi.rows();
    }

    static Matrix<double> Log(const Matrix<double>& in) {
        auto out = in;
        for(size_t i = 0; i < out.rows(); i++) {
            for(size_t j = 0; j < out.columns(); j++) { out(i, j) = log(out(i, j)); }
        }
        return out;
    }

public:
    explicit LogRegSGD(double _eta = 0.01, int iter = 10, bool _shuffle = false, int _randomState = 0)
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
        if(!sgdInit) {
            auto weightFun = [this](const Matrix<double>& x, const Matrix<double>& y) {
                return this->update_weights(x, y);
            };
            auto netInputFun = [this](const Matrix<double>& x) { return this->netInput(x); };
            sgd              = SGD(eta, n_iter, shuffle, weightFun, netInputFun);
            sgdInit          = true;
        }
        initialize_weights(X.columns());
        sgd.fit(X, y, weights);
    }


    void partial_fit(const Matrix<double>& X, const Matrix<double>& y) {
        if(!w_initialized) { initialize_weights(X.columns()); }
        sgd.partial_fit(X, y, weights);
    }

    Matrix<double> netInput(const Matrix<double>& X) override { return Sigmoid(X * weights); }

    Matrix<double> activation(const Matrix<double>& X) override { return netInput(X); }

    Matrix<double> predict(const Matrix<double>& X) override {
        std::function<bool(double)> condition = [](double x) {
            std::cout << x << std::endl;
            return bool(x >= EPS);
        };
        return where(condition, activation(X), { { 1 } }, { { -1 } });
        //        return activation(X);
    }

    virtual double costFunction([[maybe_unused]] const Matrix<double>& mat) override { return 0; }
};