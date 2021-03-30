#pragma once

class Classifier
{
protected:
    double eta; // Learning rate
    int n_iter; // number epochs
    bool w_initialized = false;

public:
    MatrixDS<double> weights; // Vector holding weights

    Classifier(double _eta, int _n_iter)
        : eta(_eta)
        , n_iter(_n_iter) { }

    void initialize_weights(size_t m) {
        weights       = MatrixDS<double>(0, m + 1, 1);
        w_initialized = true;
    }

    /**
     *
     * @param X: array-like with the shape: [n_samples, n_features]
     * @param y: array-like with shape: [n_samples, 1]
     * @return this
     */
    virtual void fit(const MatrixDS<double>&, const MatrixDS<double>&) = 0;
    virtual MatrixDS<double> predict(const MatrixDS<double>&)          = 0;
    virtual MatrixDS<double> net_input(const MatrixDS<double>&)        = 0;
};