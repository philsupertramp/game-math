#pragma once

#include "MatrixDS.h"

class AdalineGD{
    double eta; // Learning rate
    int n_iter; // number epochs
public:
    MatrixDS<double> weights; // Vector holding weights
    MatrixDS<int> costs; // Vector holding classification error per epoch

public:
    explicit AdalineGD(double _eta = 0.01, int iter = 10)
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
        costs = MatrixDS<int>(0, n_iter, 1);
        for(int iter = 0; iter < n_iter; iter++){
            auto output = net_input(X);
            auto errors = y - output;

            auto delta = (X.Transpose() * errors) * eta;
            for(size_t i = 0; i < weights.rows(); i++){
                for(size_t j = 0; j < weights.columns(); j++){
                    if(i == 0) weights[i][j] += eta * errors.sumElements();
                    else weights[i][j] += delta[i-1][j];
                }
            }
            auto cost = errors.HadamardMulti(errors).sumElements() / 2.0;
            costs[iter][0] = cost;
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
        B = MatrixDS<double>(B[0][0], X.rows(), 1);
        return (X * A) + B;
    }

    MatrixDS<double> activation(MatrixDS<double> X){
        return net_input(X);
    }

    MatrixDS<double> predict(const MatrixDS<double>& X){
        std::function<bool(double)> condition = [](double x){ return bool(x >= 0.0); };
        return where(condition, activation(X), { { 1 } }, { { -1 } });
    }
};