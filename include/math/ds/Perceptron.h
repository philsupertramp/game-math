#pragma once

#include "MatrixDS.h"

/**
 * TODO: Create a pure virtual classifier base class
 */
class Perceptron
{
    double eta; // Learning rate
    int n_iter; // number epochs
public:
    MatrixDS<double> weights; // Vector holding weights
    MatrixDS<int> errors; // Vector holding classification error per epoch

public:
    explicit Perceptron(double _eta = 0.01, int iter = 10)
    : eta(_eta), n_iter(iter)
    { }

    /**
     *
     * @param X: array-like with the shape: [n_samples, n_features]
     * @param y: array-like with shape: [n_samples, 1]
     * @return this
     */
    void fit(const MatrixDS<double>& X, const MatrixDS<double>& y){
        weights = MatrixDS<double>(0, 1 + X.columns(), 1);
        errors = MatrixDS<int>(0, n_iter, 1);
        for(int iter = 0; iter < n_iter; iter++){
            int _errors = 0;
            auto iterable = zip(X, y);
            for(const auto& elem : iterable){
                auto xi = elem.first;
                auto target = elem.second;

                auto update = (target - predict(xi)) * eta;
                auto delta = (update * xi).Transpose();

                for(size_t i = 0; i < weights.rows(); i++){
                    for(size_t j = 0; j < weights.columns(); j++){
                        if(i == 0) weights[i][j] += update[i][j];
                        else weights[i][j] += delta[i-1][j];
                    }
                }
                _errors += (int)(update[0][0] != 0.0);
            }
            errors[iter][0] = _errors;
        }
    }

    MatrixDS<double> net_input(const MatrixDS<double>& X){
        MatrixDS<double> A, B;
        A.Resize(weights.rows() - 1, weights.columns());
        B.Resize(1, weights.columns());
        for(size_t i = 0; i < weights.rows(); i++){
            for(size_t j = 0; j < weights.columns(); j++){
                if(i == 0){
                    B[i][j] = weights[i][j];
                } else {
                    A[i-1][j] = weights[i][j];
                }
            }
        }
        return (X * A) + B;
    }

    MatrixDS<double> predict(const MatrixDS<double>& X){
        std::function<bool(double)> condition = [](double x){ return bool(x >= 0.0); };
        return where(condition, net_input(X), { { 1 } }, { { -1 } });
    }
};