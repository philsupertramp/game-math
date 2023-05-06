#pragma once

#include "../Matrix.h"

double relu(const double& in) { return std::max(0.0, in); }
double sigmoid(const double& in) { return 1.0 / (1.0 + exp(-in)); }

Matrix<double> Sigmoid(const Matrix<double>& in) {
  // real sigmoid in lambda
  return in.Apply([](const double& a) { return sigmoid(a); });
}