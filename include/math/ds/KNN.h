#include "Classifier.h"
#include "../numerics/utils.h"
#include <cstddef>


class KNN
  : public Classifier
{
    size_t nearest_neighbors;
  public:
    KNN(size_t neighbors)
    : Classifier()
    , nearest_neighbors(neighbors)
    {}


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
        auto dists = zeros(x.rows(), weights.rows() - 1);
        auto trainX = weights.GetSlice(0, weights.rows() - 1, 0, weights.columns() - 2);
        auto trainY = weights.GetSlice(0, weights.rows() - 1, weights.columns() - 1, weights.columns() - 1);
        auto labels = unique(trainY);
        for(size_t i = 0; i < x.rows(); ++i){
          auto xi = x.GetSlice(i);
          std::cout << "DIFF: " << norm(trainX - xi, 0) << std::endl;
          auto current_dists = norm(trainX - xi, 0).Transpose();
          std::cout << "Sorted diffs: " << argsort(current_dists) << std::endl;
          auto gamma = zerosV(labels.rows());
          for(size_t k = 0; k < labels.rows(); ++k){
            std::function<bool(double)> condition = [i](double x) { return bool(x == i); };
            // O(N)
            auto indices = where_true(where(condition, trainY, {{1}}, {{0}})).GetSlice(0, nearest_neighbors - 1);
            auto Nk = indices.rows();
            
          }
          dists.SetSlice(i, norm(trainX - xi, 0).Transpose());
        }
        std::cout << "DISTS\n" << dists << std::endl;
        return predictions;
    };

};  

