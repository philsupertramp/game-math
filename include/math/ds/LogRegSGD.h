#pragma once

#include "MatrixDS.h"

class LogRegSGD{
    double eta; // Learning rate
    int n_iter; // number epochs
public:
    MatrixDS<double> weights; // Vector holding weights
    MatrixDS<double> costs; // Vector holding classification error per epoch
    bool shuffle = false;
    bool w_initialized = false;
    int randomState;

private:

    void initialize_weights(size_t m){
        weights = MatrixDS<double>(0, m, 1);
        w_initialized = true;
    }


    /**
     * adalineSGD: sum((target - output))^2)/2
     * logistical regression:
     */
    double update_weights(const MatrixDS<double> &xi, const MatrixDS<double> &target){
        auto output = net_input(xi);
        auto error = target - output;
        auto class1_cost = (target * -1.0) * Log(output);
        auto class2_cost = (MatrixDS<double>(1, output.rows(), output.columns()) - target) * Log(MatrixDS<double>(1, output.rows(), output.columns()) -output);
        auto cost = class1_cost - class2_cost;

        auto gradient = (xi.Transpose() * error) * eta;
        gradient = gradient * (1.0/xi.rows());

        weights = weights - gradient;

        return cost.sumElements() / xi.rows();
    }

    static MatrixDS<double> Log(const MatrixDS<double> &in){
        auto out = in;
        for(size_t i = 0; i < out.rows(); i++){
            for(size_t j = 0; j < out.columns(); j++){
                out[i][j] = log(out[i][j]);
            }
        }
        return out;
    }

    static MatrixDS<double> Sigmoid(const MatrixDS<double> &in){
        auto out = in;
        for(size_t i = 0; i < out.rows(); i++){
            for(size_t j = 0; j < out.columns(); j++){
                // real sigmoid
                out[i][j] = 1.0 / (1.0 + exp(-out[i][j]));
            }
        }
        return out;
    }

public:
    explicit LogRegSGD(double _eta = 0.01, int iter = 10, bool _shuffle = false, int _randomState = 0)
    : eta(_eta), n_iter(iter), shuffle(_shuffle), randomState(_randomState)
    {
        if(randomState != 0) Random::SetSeed(randomState);
    }

    /**
     *
     * @param X: array-like with the shape: [n_samples, n_features]
     * @param y: array-like with shape: [n_samples, 1]
     * @return this
     */
    void fit(const MatrixDS<double>& X, const MatrixDS<double>& y){
        initialize_weights(X.columns());
        costs = MatrixDS<double>(0, n_iter, 1);
        auto xCopy = X;
        auto yCopy = y;
        for(int iter = 0; iter < n_iter; iter++){
            if(shuffle){
                auto fooPair = shuffleData(X, y);
                xCopy = fooPair.first;
                yCopy = fooPair.second;
            }
            double costSum = 0;
            for(const auto &elem : zip(xCopy, yCopy)){
                costSum += update_weights(elem.first, elem.second);
            }

            costs[iter][0] = costSum/yCopy.rows();
        }
    }


    void partial_fit(const MatrixDS<double> &X, const MatrixDS<double> &y){
        if(!w_initialized){
            initialize_weights(X.columns());
        }
        if(y.rows() > 1){
            for(const auto& elem : zip(X, y)){
                update_weights(elem.first, elem.second);
            }
        } else {
            update_weights(X, y);
        }
    }

    std::pair<MatrixDS<double>, MatrixDS<double>> shuffleData([[maybe_unused]] const MatrixDS<double>& X, [[maybe_unused]] const MatrixDS<double>& y){
        // TODO
    }

    MatrixDS<double> net_input(const MatrixDS<double>& X){
        return Sigmoid(X * weights);
    }

    MatrixDS<double> activation(const MatrixDS<double>& X){
        return net_input(X);
    }

    MatrixDS<double> predict(const MatrixDS<double>& X){
//        std::function<bool(double)> condition = [](double x){ return bool(x >= 0.0); };
        return activation(X); //where(condition, activation(X), { { 1 } }, { { -1 } });
    }
};