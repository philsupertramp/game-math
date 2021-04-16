/**
 * Regular backward substitution.
 * Calculates result given upper right triangular matrix z=R*b => R^-1b = z
 */

#pragma once
#include "../utils.h"
#include <iostream>
#include <vector>


Matrix<double> backwardSub(const Matrix<double>& R, const Matrix<double>& b);
