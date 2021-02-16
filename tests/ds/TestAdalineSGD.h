#pragma once


#include "../Test.h"
#include <cassert>
#include <math/ds/DataSet.h>
#include <math/ds/AdalineSGD.h>


bool TestAdalineSGD() {
    auto ppn = AdalineSGD(0.01, 15, false, 1);
    Set ds("../../resources/iris_data_files/iris.data", 2, 1);
//    ds.Normalize(NormalizerMethod::COL_MEAN);
    ppn.fit(ds.Input, ds.Output);
    std::cout << ppn.weights;
    assert(ppn.predict({{ 5.4, 0.2 }})[0][0] == -1); // == -1
    // [-0.16661009 -0.19663501  0.58227774]
    std::cout << ppn.weights;
    return true;
}