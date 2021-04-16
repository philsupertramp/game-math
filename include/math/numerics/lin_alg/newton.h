#pragma once
#include "../utils.h"
#include "gaussSeidel.h"
#include <functional>


using Jacobian       = std::function<Matrix<double>(double, const Matrix<double>&)>;
using LinearEquation = std::function<Matrix<double>(double, const Matrix<double>&)>;

Matrix<double> newton(
const std::function<Matrix<double>(const Matrix<double>&)>& f,
const std::function<Matrix<double>(const Matrix<double>&)>& Df,
const Matrix<double>& x0,
double TOL,
int maxIter);
