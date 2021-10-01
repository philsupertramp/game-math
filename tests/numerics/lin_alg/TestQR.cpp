#include "../../Test.h"
#include <math/numerics/lin_alg/qr.h>
#include <math/numerics/utils.h>


class SVDTestCase : public Test
{
    bool TestQR() {
        Matrix<double> A = { { 2, 2, 8 }, { 5, 6, 6 }, { 4, 4, 8 } };
        Matrix<double> R = { { -6.708203932499369e+00, -7.453559924999299e+00, -1.162755348299891e+01 },
                             { 0, -6.666666666666672e-01, 4.000000000000002e+00 },
                             { 0, 0, 3.577708763999661e+00 } };
        Matrix<double> Q = {
            { -2.981423969999721e-01, 3.333333333333336e-01, 8.944271909999157e-01 },
            { -7.453559924999299e-01, -6.666666666666666e-01, 2.220446049250313e-16 },
            { -5.962847939999439e-01, 6.666666666666666e-01, -4.472135954999583e-01 },
        };
        //
        //        R *= -1.0;
        //        Q *= -1.0;
        auto result = qr(A);
        std::cout << "R:\n" << R << "\nQ:\n" << Q << std::endl;
        std::cout << "vs\nR:\n" << result.second << "\nQ:\n" << result.first << std::endl;
        // A stays A
        AssertEqual(Q, result.first);
        AssertEqual(R, result.second);

        return true;
    }

public:
    void run() override { TestQR(); }
};

int main() {
    SVDTestCase().run();
    return 0;
}