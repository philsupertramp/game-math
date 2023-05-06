#pragma once
#include "../../Matrix.h"
#include "../../statistics/Probability.h"


class StandardScaler
{
    Matrix<double> means;
    Matrix<double> std_deviations;
    bool with_std = true;
    bool with_means = true;

  public:
    StandardScaler(bool withStd = true, bool withMeans = true)
    : with_std(withStd)
    , with_means(withMeans)
    {};

    void fit(const Matrix<double>& in){
      means = mean(in, 0);
      std_deviations = sd(in, 0);
    }

    Matrix<double> transform(const Matrix<double>& in){
      return (in - means)* (1.0/std_deviations);
    }
};
