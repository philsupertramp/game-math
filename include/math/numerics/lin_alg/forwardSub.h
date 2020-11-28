/**
 * Regular forward substitution.
 * Calculates result given lower left triangular matrix b=L*c => L^-1b = c
 */

#pragma once
#include "../utils.h"
#include <iostream>
#include <vector>


std::vector<float> forwardSub(const std::vector<std::vector<float>>& L, const std::vector<float>& b);
