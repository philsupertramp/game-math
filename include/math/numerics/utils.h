#pragma once
#include <vector>

#ifndef EPS
    #define EPS 1e-5
#endif


std::vector<float> linspace(float start, float end, unsigned long num_elements);

std::vector<std::vector<float>> zeros(size_t rows, size_t columns);

std::vector<std::vector<float>> ones(size_t rows, size_t columns = 1);

/**
 * creates unit matrix of dimension rows, columns
 * @param rows
 * @param columns if 0, then columns = rows
 * @return
 */
std::vector<std::vector<float>> eye(size_t rows, size_t columns = 0);

std::vector<float> zerosV(size_t rows);

double norm(const std::vector<std::vector<float>> &in);

std::vector<float> newton(std::vector<std::vector<float>>& A, std::vector<float>& b);