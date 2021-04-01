#include "utils.h"
#include "lin_alg/LU.h"
#include "lin_alg/backwardSub.h"
#include "lin_alg/forwardSub.h"
#include <cmath>

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
std::vector<std::vector<float>> ones(size_t rows, size_t columns){
    return std::vector<std::vector<float>>(rows, std::vector<float>(columns, 1));
}

std::vector<std::vector<float>> eye(size_t rows, size_t columns){
    auto realCols = columns;
    if(columns == 0) realCols = rows;
    auto out = zeros(rows, realCols);
    for(size_t i = 0; i < rows; ++i){
        out[i][i] = 1.0;
    }
    return out;
}
double norm(const std::vector<std::vector<float>> &in){
    double out = 0;
    for(const auto & i : in){
        for(float j : i) {
            out += j * j;
        }
    }
    return sqrt(out);
}
std::vector<float> zerosV(size_t rows) { return std::vector<float>(rows, 0.0f); }

std::vector<float> newton(std::vector<std::vector<float>>& A, std::vector<std::vector<float>>& b){
    auto foo = LU(A);
    A = foo.first;
    int index = 0;
    for(auto i : foo.second){
        b[0][index] = b[0][i];
        index += 1;
    }
    auto c = forwardSub(A, b[0]);
    auto x = backwardSub(A, c);

    return x;

}