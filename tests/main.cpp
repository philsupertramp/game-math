#include "vec/TestVec2.h"
#include "vec/TestVec3.h"
#include "vec/TestVec4.h"
#include "mat/TestMat2.h"
#include "mat/TestMat3.h"
#include "mat/TestMat4.h"

int main(int nargs, char* argv[]) {
    TestVec2();
    TestVec3();
    TestVec4();

    TestMat2();
    TestMat3();
    TestMat4();

    return 0;
}
