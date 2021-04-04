#pragma once
#include "../utils.h"
#include "gaussSeidel.h"
#include <functional>


using Jacobian = std::function<std::vector<std::vector<float>>(float, std::vector<float>)>;
using LinearEquation = std::function<std::vector<float>(float, std::vector<float>)>;

std::vector<float> newton(
const std::function<std::vector<float>(std::vector<float>)>& f,
const std::function<std::vector<std::vector<float>>(std::vector<float>)>& Df,
const std::vector<float>& x0,
float TOL,
int maxIter);
