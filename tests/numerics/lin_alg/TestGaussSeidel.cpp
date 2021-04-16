#include "../../Test.h"
#include <math/numerics/lin_alg/gaussSeidel.h>
#include <vector>

class GaussSeidelTestCase : public Test
{
    bool TestGaussSeidel() {
        Matrix<double> A({ { 4, 2, 3 }, { 2, 2, 1 }, { 2, 2, 2 } });
        Matrix<double> b({ { 2 }, { 1 }, { 2 } });

        auto result = gaussSeidel(A, b);

        std::vector<double> expectedX = { -0.5f, 0.5f, 1.0f };
        for(size_t i = 0; i < expectedX.size(); i++) { AssertEqual(result(i, 0), expectedX[i]); }
        return true;
    }
    bool TestGaussSeidel2() {
        Matrix<double> A({ { 2, -1 }, { 1, 3 } });
        Matrix<double> b({ { 4 }, { -5 } });

        auto result = gaussSeidel(A, b);

        std::vector<double> expectedX = { 1.0f, -2.0f };
        for(size_t i = 0; i < expectedX.size(); i++) { AssertEqual(result(i, 0), expectedX[i]); }
        return true;
    }

public:
    void run() override {
        TestGaussSeidel();
        TestGaussSeidel2();
    }
};

int main() {
    GaussSeidelTestCase().run();
    return 0;
}