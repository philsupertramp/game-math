#pragma once
#include "LU.h"
#include "backwardSub.h"
#include "forwardSub.h"
#include <vector>


std::vector<float> gaussianElimination(std::vector<std::vector<float>> A, std::vector<float> b);