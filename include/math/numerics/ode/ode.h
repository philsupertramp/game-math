#pragma once

#include <functional>
#include <vector>

using ODE       = std::function<std::vector<float>(float, std::vector<float>)>;
using ODEResult = std::pair<std::vector<std::vector<float>>, std::vector<float>>;
