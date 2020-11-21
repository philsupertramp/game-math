#pragma once

#include <vector>
#include <functional>

std::vector<float> linspace(float start, float end, unsigned long num_elements){
    std::vector<float> result;
    result.resize(num_elements);
    float h = (end - start)/(float)(num_elements-1);
    result[0] = start;
    for(size_t i=1; i<num_elements; i++){
        result[i] = result[i-1]+h;
    }
    return result;
}

std::vector<std::vector<float>> zeros(size_t rows, size_t columns){
    std::vector<std::vector<float>> result;
    result.resize(rows);
    for(size_t i = 0; i<rows; i++) {
        result[i].resize(columns);
    }
    return result;
}

using ODE = std::function<std::vector<float>(float,std::vector<float>)>;
using ODEResult = std::pair<std::vector<std::vector<float>>,std::vector<float>>;
