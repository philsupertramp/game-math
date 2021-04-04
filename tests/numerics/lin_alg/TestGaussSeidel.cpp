#include "../../Test.h"
#include <math/numerics/lin_alg.h>
#include <vector>

class GaussSeidelTestCase : public Test
{
    bool TestGaussSeidel() {
        std::vector<std::vector<float>> A = { { 4, 2, 3 }, { 2, 2, 1 }, { 2, 2, 2 } };
        std::vector<float> b              = { 2, 1, 2 };

        auto result = gaussSeidel(A, b);

        std::vector<float> expectedX = { -0.5f, 0.5f, 1.0f };
        for(size_t i = 0; i < expectedX.size(); i++) { assert(result[i] == expectedX[i]); }
        return true;
    }

public:
    void run() override { TestGaussSeidel(); }
};

int main() {
    GaussSeidelTestCase().run();
    return 0;
}