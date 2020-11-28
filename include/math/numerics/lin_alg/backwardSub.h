/**
 * Regular backward substitution.
 * Calculates result given upper right triangular matrix z=R*b => R^-1b = z
 */

#pragma once
#include "../utils.h"
#include <iostream>
#include <vector>


std::vector<float> backwardSub(const std::vector<std::vector<float>>& R, const std::vector<float>& b);
