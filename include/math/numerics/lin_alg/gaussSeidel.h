#pragma once
#include "../../Matrix.h"
#include "LU.h"
#include "backwardSub.h"
#include "forwardSub.h"
#include <vector>


Matrix<double> gaussSeidel(const Matrix<double>& A, const Matrix<double>& b);