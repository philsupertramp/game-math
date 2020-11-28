#pragma once
#include <vector>

#ifndef EPS
#define EPS 1e-5
#endif


std::vector<float> linspace(float start, float end, unsigned long num_elements);

std::vector<std::vector<float>> zeros(size_t rows, size_t columns);

std::vector<float> zerosV(size_t rows);