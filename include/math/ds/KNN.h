#include "../numerics/utils.h"
#include "Classifier.h"
#include <cstddef>


class KNN : public Classifier
{
    int nearest_neighbors;

public:
    KNN(int neighbors)
        : Classifier()
        , nearest_neighbors(neighbors) { }


    /**
     *
     */
    virtual void fit(const Matrix<double>& X, const Matrix<double>& y) override {
        assert(y.rows() == 1 || y.columns() == 1);
        weights = HorizontalConcat(X, y);
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
        auto trainX      = weights.GetSlice(0, weights.rows() - 1, 0, weights.columns() - 2);
        auto trainY      = weights.GetSlice(0, weights.rows() - 1, weights.columns() - 1, weights.columns() - 1);
        auto labels      = unique(trainY);
        for(size_t i = 0; i < x.rows(); ++i) {
            auto xi            = x.GetSlice(i);
            auto current_dists = norm(trainX - xi, 0);
            auto k_neares      = trainY.GetSlicesByIndex(argsort(current_dists));
            auto k_nearest     = k_neares.GetSlice(0, nearest_neighbors - 1);
            auto gamma         = zeros(labels.rows(), 1);
            std::cout << "Based on: " << current_dists << std::endl
                      << "With order: " << argsort(current_dists) << std::endl
                      << "From dists: " << current_dists.GetSlicesByIndex(argsort(current_dists))
                      << "unordered labels: " << trainY << std::endl
                      << "k nearest: " << k_neares << std::endl;
            for(size_t k = 0; k < labels.rows(); ++k) {
                std::function<bool(double)> condition = [k](double x) { return bool(x == k); };
                // O(N)
                auto yis = where(condition, k_nearest, { { 1 } }, { { 0 } });
                // O(N)
                gamma(k, 0) = yis.sumElements();
            }
            std::cout << "elem: " << i << "\ngamma: " << gamma << std::endl;
            predictions(i, 0) = argmax(gamma);
        }
        return predictions;
    };
};
