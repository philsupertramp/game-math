#pragma once


#include "../Test.h"
#include <cassert>
#include <math/ds/DataSet.h>
#include <math/ds/AdalineGD.h>


bool TestAdalineGD() {
    auto ppn = AdalineGD(0.0001, 15);
    Set ds("../../resources/iris_data_files/iris.data", 2, 1);
    ppn.fit(ds.Input, ds.Output);
    std::cout << ppn.weights;
    assert(ppn.predict({{ 5.4, 0.2 }})[0][0] == -1); // == -1
    // [-0.01174401 -0.02122706  0.09641832]
    std::cout << ppn.weights;
    return true;
}