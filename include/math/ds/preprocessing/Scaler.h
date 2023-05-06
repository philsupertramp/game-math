#pragma once
#include "../../Matrix.h"
#include "../../statistics/Probability.h"


class StandardScaler
{
private:
  bool with_std   = true;
  bool with_means = true;

public:
  Matrix<double> means;
  Matrix<double> std_deviations;

public:
  StandardScaler(bool withStd = true, bool withMeans = true)
    : with_std(withStd)
    , with_means(withMeans){};

  void fit(const Matrix<double>& X, const Matrix<double>& y) {
    means          = mean(X, 0);
    std_deviations = sd(X, 0);
  }

  Matrix<double> transform(const Matrix<double>& in) { return (in - means) * (1.0 / std_deviations); }
};
