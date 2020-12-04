#include "utils.h"

std::vector<float> linspace(float start, float end, unsigned long num_elements) {
    std::vector<float> result;
    result.resize(num_elements);
    float h   = (end - start) / (float)(num_elements - 1);
    result[0] = start;
    for(size_t i = 1; i < num_elements; i++) { result[i] = i * h; }
    return result;
}

std::vector<std::vector<float>> zeros(size_t rows, size_t columns) {
    return std::vector<std::vector<float>>(rows, std::vector<float>(columns, 0));
}
std::vector<float> zerosV(size_t rows) { return std::vector<float>(rows, 0.0f); }
