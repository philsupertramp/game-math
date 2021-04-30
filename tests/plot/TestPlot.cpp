#include "../Test.h"
#include <math/numerics/analysis/SupportValues.h>
#include <math/visualization/Plot.h>


class PlotTestCase : public Test
{
    bool TestPlot() {
        Matrix<double> X = linspace(-5, 5, 5).Transpose();
        Matrix<double> Y = X.Apply([](const double& in) { return in * in; });

        LagrangeBase base(X, Y);
        Matrix<double> xi = linspace(-3.5, 3.5, 5).Transpose();
        auto res          = base.Evaluate(xi);

        Plot plot("Comparison of approximation with true value");

        plot.AddData(HorizontalConcat(X, Y), "support values");
        plot.AddData(HorizontalConcat(xi, res), "approximation");

        plot();

        return true;
    }

    bool TestScatterPlot() {
        Matrix<double> X = linspace(-5, 5, 5).Transpose();
        Matrix<double> Y = X.Apply([](const double& in) { return in * in; });
        ScatterPlot plot2("Data visualization");
        plot2.AddData(X, Y, "support values");
        plot2.xAxis("X");
        plot2.yAxis("Y");
        plot2();

        return true;
    }
    bool TestFunctionPlot() {
        FunctionPlot plot3([](const double& in) { return in * in; }, "x^2");
        plot3.AddData(-5, 5, 0.1, "support values");
        plot3.xAxis("X");
        plot3.yAxis("Y");
        plot3();
        return true;
    }

public:
    void run() override {
        TestPlot();
        TestScatterPlot();
        TestFunctionPlot();
    }
};

int main() { PlotTestCase().run(); }