
#include "ds/TestAdalineGD.h"
#include "ds/TestAdalineSGD.h"
#include "ds/TestMatrixDS.h"
#include "ds/TestPerceptron.h"
#include "mat/TestMat2.h"
#include "mat/TestMat3.h"
#include "mat/TestMat4.h"
#include "numerics/lin_alg/TestBackwardSub.h"
#include "numerics/lin_alg/TestForwardSub.h"
#include "numerics/lin_alg/TestGaussianElimination.h"
#include "numerics/lin_alg/TestLU.h"
#include "numerics/ode/TestExplicitEuler.h"
#include "numerics/ode/TestODE45.h"
#include "vec/TestVec2.h"
#include "vec/TestVec3.h"
#include "vec/TestVec4.h"

#define PLATFORM_UNIX

int main([[maybe_unused]] int nargs, [[maybe_unused]] char* argv[]) {
#ifndef PLATFORM_UNIX
    Magick::InitializeMagick(*argv);
#endif
    TestVec2();
    TestVec3();
    TestVec4();

    TestMat2();
    TestMat3();
    TestMat4();

    TestExplicitEuler();
    TestExplicitEulerRB();

    TestOde45();
    TestOde45RB();

    TestBackwardSub();

    TestForwardSub();

    TestLU();

    TestGaussianElimination();

    TestMatrixDS();


    TestPerceptron();
    TestAdalineGD();
    TestAdalineSGD();
    return 0;
}
