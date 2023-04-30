/**
 * Nearest Centroid Classifier:
 *
 * Linear classifier for linear seperable/uncorrelated data.
 *
 * Prediction:
 *   In:
 *     $x$: Data point to classify
 *     $\mu_k$: $k$ class centroids
 *   Out: $$
 *     argmin(||\mu_k - x||_2)
 *   $$
 *
 * Compute centroids/fit model:
 *
 *   Batch-Mode:
 *     In:
 *       $x_i$ with $x_i\in \R^D$: $N$ training samples
 *       $y_i$ with $y_i\in \{1, ..., K\}$: $N$ training sample lables
 *     Out:
 *       $\mu_k$: $k$ class centroids
 *
 *     Computes **all** centroids at once, holds $N$ $D$-dimensional samples in memory at-a-time.
 *
 *   Iterative/Streaming-Mode:
 *     In:
 *       $x_i$ with $x_i\in \R^D$: $N$ training samples
 *       $y_i$ with $y_i\in \{1, ..., K\}$: $N$ training sample lables
 *     Out:
 *       $\mu_k$: $K$ class centroids
 *
 *     Calculates centroids iteratively, holds $K$ centroids and 1 training sample in memory at-a-time.
 */

#include "../numerics/utils.h"
#include "../utils.h"
#include "Classifier.h"
#include <cstddef>


class NCC : public Classifier
{
public:
    NCC()
        : Classifier() { }

    /**
     *
     */
    virtual void fit(const Matrix<double>& X, const Matrix<double>& y) override {
        assert(y.rows() == 1 || y.columns() == 1);
        auto labels   = unique(y);
        weights       = zeros(labels.rows(), X.columns());
        auto counters = Matrix<int>(0, labels.rows(), 1);
        for(size_t i = 0; i < X.rows(); ++i) {
            auto k               = y(i, 0);
            auto xi              = X.GetSlice(i, i);
            auto current_counter = counters(k, 0);
            weights.SetSlice(
            k,
            weights.GetSlice(k, k) * (current_counter / double(current_counter + 1))
            + xi * (1. / double(current_counter + 1)));
            counters(k, 0) += 1;
        }
    };


    /**
      Prediction:
  *   In:
  *     $x$: Data point to classify
  *     $\mu_k$: $k$ class centroids
  *   Out: $$
  *     argmin(||\mu_k - x||_2)
  *   $$
  *
      *
      *
      */
    virtual Matrix<double> predict(const Matrix<double>& x) override {
        auto predictions = zerosV(x.rows());
        for(size_t i = 0; i < x.rows(); ++i) {
            auto xi           = x.GetSlice(i, i);
            auto distances    = norm(weights - xi, 0);
            predictions(i, 0) = argmin(distances);
        }
        return predictions;
    };
};
