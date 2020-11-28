#include "LU.h"

std::pair<std::vector<std::vector<float>>, std::vector<unsigned int>> LU(std::vector<std::vector<float>> &A)
{
    auto m = A.size();
    auto n = A[0].size();

    if(m != n)
    {
        // Error
    }

    // init pivot vector
    std::vector<unsigned int> p(m, 0);
    for(size_t i = 0; i < m; i++) { p[i] = i; }

    for(size_t col = 0; col < n-1; col++){
        auto maxVal = std::abs(A[col][col]);
        auto index = col;
        for(size_t q = col; q < n; q++){
            if(std::abs(A[q][col]) > maxVal){
                maxVal = std::abs(A[q][col]);
                index = q;
            }
        }
        auto safe = p[index];
        p[index] = p[col];
        p[col] = safe;

        std::vector<float> matrixSafe = A[col];
        A[col] = A[index];
        A[index] = matrixSafe;

        for(size_t row = col+1; row < n; row++){
            A[row][col] = A[row][col] / A[col][col];
        }

        for(size_t i = col + 1; i < m; i++){
            for(size_t j = col + 1; j < n; j++){
                A[i][j] = A[i][j] - A[i][col] * A[col][j];
            }
        }
    }

    return { A, p };
}