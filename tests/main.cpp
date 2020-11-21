#include "vec/TestVec2.h"
#include "vec/TestVec3.h"
#include "vec/TestVec4.h"
#include "mat/TestMat2.h"
#include "mat/TestMat3.h"
#include "mat/TestMat4.h"
#include "ode/TestExplicitEuler.h"
#include "ode/TestODE45.h"

int main([[maybe_unused]] int nargs,[[maybe_unused]] char* argv[]) {
    TestVec2();
    TestVec3();
    TestVec4();

    TestMat2();
    TestMat3();
    TestMat4();
    TestExplicitEuler();
    TestExplicitEulerRB();
    TestOde45();
    return 0;
}
