#pragma once

#include "math/Matrix.h"


/**
 * Classifier template class
 *
 * Inherit and implement:
 * - fit: method used to train the classifier
 *      - initializes weights
 *      - cycles through epochs
 *          - updates weights
 *          - calculates cost of current epoch
 * - predict: method used to predict the output for given input
 * - activation: method to activate a given input
 * - netInput: converts activated output into net output
 * - costFunction: method to calculate the current cost
 */
class Classifier
{
protected:
    //! Learning rate
    double eta;
    //! number epochs
    int n_iter;
    //! flag to initialize weights only once
    bool w_initialized = false;

public:
    //! Vector holding weights
    Matrix<double> weights;
    //! Vector holding classification error per epoch
    Matrix<double> costs;

    /**
     * default constructor
     * @param _eta learning rate
     * @param _n_iter number learning iterations
     */
    Classifier(double _eta, int _n_iter)
        : eta(_eta)
        , n_iter(_n_iter) { }

    /**
     * Initialization method for weights hold by the classifier
     * @param numRows number rows
     * @param numColumns number columns
     */
    void initialize_weights(size_t numRows, size_t numColumns = 1) {
        weights       = Matrix<double>(0, numRows + 1, numColumns);
        w_initialized = true;
    }

    /**
     * Implements training algorithm
     * @param X: array-like with the shape: [n_samples, n_features]
     * @param y: array-like with shape: [n_samples, 1]
     * @return this
     */
    virtual void fit(const Matrix<double>& X, const Matrix<double>& y) = 0;

    /**
     * Makes prediction for given input
     * @return
     */
    virtual Matrix<double> predict(const Matrix<double>&) = 0;

    /**
     * Activates a given input
     * @return
     */
    virtual Matrix<double> activation(const Matrix<double>&) = 0;

    /**
     * calculates the net-input (aka output) this is mostly the derivative of `activation`
     * @return
     */
    virtual Matrix<double> netInput(const Matrix<double>&) = 0;

    /**
     * calculates the current cost
     * if unused override returning 0
     */
    virtual double costFunction(const Matrix<double>&) = 0;
};