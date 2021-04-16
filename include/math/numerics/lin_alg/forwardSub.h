/**
 * Regular forward substitution.
 * Calculates result given lower left triangular matrix b=L*c => L^-1b = c
 */

#pragma once
#include "../utils.h"
#include <iostream>
#include <vector>


Matrix<double> forwardSub(const Matrix<double>& L, const Matrix<double>& b);
