#pragma once


#include "../Test.h"
#include <cassert>
#include <math/ds/DataSet.h>
#include <math/ds/Perceptron.h>


bool TestPerceptron() {
    auto ppn = Perceptron(0.1, 10);
    Set ds("../../resources/iris_data_files/iris.data", 2, 1);
    ppn.fit(ds.Input, ds.Output);
    assert(ppn.predict({{ 5.4, 0.2 }})[0][0] == -1); // == -1
    //
    return true;
}