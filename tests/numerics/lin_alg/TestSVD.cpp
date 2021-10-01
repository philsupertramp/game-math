#include "../../Test.h"
#include <math/numerics/lin_alg/svd.h>
#include <math/numerics/utils.h>


class QRTestCase : public Test
{
    bool TestSVD() {
        Matrix<double> A = { { 1, 1, 1, 1 }, { 1, 3, 1, 2 } };
        auto result      = svd(A, 0);
        auto U           = result[0];
        auto S           = eye(U.rows(), U.columns());
        S(0, 0)          = result[1](0, 0);
        S(1, 1)          = result[1](0, 1);
        auto VH          = result[2];
        // A stays A
        AssertEqual(U * S * VH, A);

        return true;
    }

public:
    void run() override { TestSVD(); }
};

int main() {
    QRTestCase().run();
    return 0;
}