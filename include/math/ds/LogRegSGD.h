#pragma once

#include "../Matrix.h"
#include "Classifier.h"
#include "utils.h"

/**
 * Logistical regression model using statistic gradient decent method for model fitting.
 */
class LogRegSGD : public Classifier
{
public:
    //! signalizes whether given dataset should be shuffled while fitting
    bool shuffle;
    //! initialize weights with random state
    int randomState;
    //! algorithmic object to represent fitting algorithm
    SGD* sgd = nullptr;

private:
    /**
     * helper to initialize weights matrix
     * @param m
     */
    void initialize_weights(size_t m) {
        if(randomState) weights = Matrix<double>::Random(m, 1);
        else weights       = Matrix<double>(0, m, 1);
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

    /**
     * Calculates log of each element
     * @param in
     * @return
     */
    static Matrix<double> Log(const Matrix<double>& in) {
        auto out = in;
        for(size_t i = 0; i < out.rows(); i++) {
            for(size_t j = 0; j < out.columns(); j++) { out(i, j) = log(out(i, j)); }
        }
        return out;
    }

public:
    /**
     * default constructor
     * @param _eta
     * @param iter
     * @param _shuffle
     * @param _randomState
     */
    explicit LogRegSGD(double _eta = 0.01, int iter = 10, bool _shuffle = false, int _randomState = 0)
        : Classifier(_eta, iter)
        , shuffle(_shuffle)
        , randomState(_randomState) {
        if(randomState != 0) Random::SetSeed(randomState);
    }

    /**
     * fits the weights of the model for given input values
     * @param X: array-like with the shape: [n_samples, n_features]
     * @param y: array-like with shape: [n_samples, 1]
     * @return this
     */
    void fit(const Matrix<double>& X, const Matrix<double>& y) override {
        if(sgd == nullptr) {
            std::function<double(const Matrix<double>&, const Matrix<double>&)> weightFun = [this](const Matrix<double>& x, const Matrix<double>& y) {
                return this->update_weights(x, y);
            };
            std::function<Matrix<double>(const Matrix<double>&)> netInputFun = [this](const Matrix<double>& x) { return this->netInput(x); };
            sgd              = new SGD(eta, n_iter, shuffle, weightFun, netInputFun);
        }
        initialize_weights(X.columns());
        sgd->fit(X, y, weights);
    }


    /**
     * partially fit weights
     * @param X
     * @param y
     */
    void partial_fit(const Matrix<double>& X, const Matrix<double>& y) {
        if(!w_initialized) { initialize_weights(X.columns()); }
        sgd->partial_fit(X, y, weights);
    }

    /**
     * calculate netinput
     * @param X
     * @return
     */
    Matrix<double> netInput(const Matrix<double>& X) override { return Sigmoid(X * weights); }

    /**
     * activate given input
     * @param X
     * @return
     */
    Matrix<double> activation(const Matrix<double>& X) override { return netInput(X); }

    /**
     * predict output for given input
     * @param X
     * @return
     */
    Matrix<double> predict(const Matrix<double>& X) override {
        std::function<bool(double)> condition = [](double x) {
            return bool(x >= EPS);
        };
        return where(condition, activation(X), { { 1 } }, { { -1 } });
        //        return activation(X);
    }

    /**
     * do not use!
     * @param mat
     * @return
     */
    virtual double costFunction([[maybe_unused]] const Matrix<double>& mat) override { return 0; }
};