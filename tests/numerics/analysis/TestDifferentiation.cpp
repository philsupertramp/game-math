#include "../../Test.h"
#include <math/numerics/analysis/Differentiation.h>
#include <math/visualization/Plot.h>


class DifferentiationTestCase : public Test
{
    int m = 15;
    double a = 0;
    double b = 2*M_PI;
    Matrix<double> xk = linspace(a, b, m).Transpose();

    Matrix<double> f(const Matrix<double>& in){
        return in.Apply([](const double& val){return cos(val);});
    }

    Matrix<double> df(const Matrix<double>& in){
        return in.Apply([](const double& val){return -sin(val); });
    }


    bool TestDiffs(){
        auto res = forwardDiff(xk, f(xk));
        Plot plot("diffs");
        plot.AddData(HorizontalConcat(xk, df(xk)), "Analytical result");
        plot.AddData(HorizontalConcat(xk, res), "forward", DataTypes::DOTS);
        plot.AddData(HorizontalConcat(xk,backwardDiff(xk, f(xk))), "backward", DataTypes::DOTS);
        plot.AddData(HorizontalConcat(xk,backwardDiff2(xk, f(xk))), "backward2", DataTypes::DOTS);
        plot.AddData(HorizontalConcat(xk,centralDiff(xk, f(xk))), "centralDiff", DataTypes::DOTS);
        plot.AddData(HorizontalConcat(xk,centralDiff4(xk, f(xk))), "centralDiff4", DataTypes::DOTS);

        plot();
        return true;
    }

public:
    virtual void run() {
        TestDiffs();
    }
};


int main() {
    DifferentiationTestCase().run();
    return 0;
}