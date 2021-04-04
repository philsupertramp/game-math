#include "../../Test.h"
#include <math/numerics/lin_alg.h>
#include <vector>
#include <cmath>


class NewtonTestCase : public Test
{
    bool TestNewton() {
//        auto f = [](std::vector<float> x){
//            return std::vector<float>{atanf(x[0]*x[0]+x[1]*x[1]), atanf(x[0]*x[0]-x[1]*x[1])};
//        };
//        auto Df = [](const std::vector<float>& x){
//            return std::vector<std::vector<float>>{ {
//                static_cast<float>((2.0 * x[0]) / ((x[0] * x[0] + x[1] * x[1]) * (x[0] * x[0] + x[1] * x[1]) + 1.0)),
//                static_cast<float>((2.0 * x[1]) / ((x[0] * x[0] + x[1] * x[1]) * (x[0] * x[0] + x[1] * x[1]) + 1.0)),
//                }, {
//                static_cast<float>((2.0 * x[0]) / ((x[0] * x[0] - x[1] * x[1]) * (x[0] * x[0] - x[1] * x[1]) + 1.0)),
//                static_cast<float>((-2.0 * x[1]) / ((x[0] * x[0] - x[1] * x[1]) * (x[0] * x[0] - x[1] * x[1]) + 1.0)),
//            }};
//        };
        auto f = [](std::vector<float> x) {
            return std::vector<float>({ cosf(x[0]) - x[0] });
        };
        auto Df = [](std::vector<float> x) {
            return std::vector<std::vector<float>>({ { (-sinf(x[0]) - 1) } });
        };

        int maxIter = 100;
        float TOL = 1e-10;
        std::vector<float> x0({1});
        auto res = newton(f, Df, x0, TOL, maxIter);

        std::vector<float> xExpt = {
            0.7390851332
        };
        for(int i = 0; i < 1; ++i) {
            assert(res[i] == xExpt[i]);
        }
        return true;
    }

public:
    void run() override {
        TestNewton();
    }
};

int main() { NewtonTestCase().run(); }