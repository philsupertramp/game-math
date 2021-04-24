#include "../Test.h"
#include <math/plot/plot.h>


class PlotTestCase : public Test
{
    bool TestPlot() {

        FILE *gnuplot = popen("gnuplot --persist", "w");
        char* plotNames[1] = {"Test123"};
        plotAttributes attrs = {"x^2", "X", "Y", 0, 10, 0, "#008080", 0, gnuplot, &plotNames[0]};
        plotFun([](const double& in){ return pow(in, 2); }, -10, 10, -10, 10, 0.1, attrs);

        FILE *gnuplot2 = popen("gnuplot --persist", "w");
        plotAttributes attrs2 = {"x+2", "X", "Y", 0, 10, 0, "#008080", 0, gnuplot2, &plotNames[0]};
        Matrix<double> A(0, 10, 1);
        for(size_t i = 0; i < 10; ++i){
            A(i, 0) = i;
        }
        auto B = A.Apply([](const double& x){return x + 2;});
        multiPlot(HorizontalConcat(A, B), attrs2);
        return true;
    }

public:
    void run() override { TestPlot(); }
};

int main() { PlotTestCase().run(); }