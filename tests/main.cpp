#include "mat/TestMat2.h"
#include "mat/TestMat3.h"
#include "mat/TestMat4.h"
#include "vec/TestVec2.h"
#include "vec/TestVec3.h"
#include "vec/TestVec4.h"

#include <iomanip>
#include <iostream>

int main(int nargs, char* argv[])
{
    size_t count     = 100;
    double acc_times = 0;
    double acc_add   = 0;
    double minVal    = 1;
    double maxVal    = 2000000;
    double dx        = (maxVal - minVal) / (count - 1);

    acc_add = minVal;
    for(size_t i = 0; i < count; i++)
    {
        acc_times   = minVal + i * dx;
        double diff = acc_times - acc_add;

        if(diff != 0) std::cout << " diff ";
        else
            std::cout << "      ";
        std::cout << "i " << i << " diff:" << std::scientific << std::setprecision(17) << diff << " dx " << dx
                  << " acc_times " << acc_times << " acc_add " << acc_add << '\n';

        acc_add += dx;
    }
    return 0;


    TestVec2();
    TestVec3();
    TestVec4();

    TestMat2();
    TestMat3();
    TestMat4();

    return 0;
}
