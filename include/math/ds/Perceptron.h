#pragma once

class Perceptron
{
    float eta; // Learning rate
    int n_iter; // number epochs
    MatrixDS<float> weights;
    std::vector<int> errors;

public:
    Perceptron(float _eta=0.01, int iter = 10)
    : eta(_eta), n_iter(iter)
    { }

    /**
     *
     * @param X: array-like with the shape: [n_samples, n_features]
     * @param y: array-like with shape: [n_samples, 1]
     * @return this
     */
    Perceptron fit(MatrixDS<float> X, MatrixDS<float> y){
        // TODO: math-magic
        return this;
    }

    MatrixDS<float> net_input(MatrixDS<float> X){
        return X.dot() + MatrixDS<float>(weights[0]);
    }

    MatrixDS<float> predict(MatrixDS<float> X){
        // np.where(net_input(X) >= 0.0, 1.0, -1.0)

    }
};