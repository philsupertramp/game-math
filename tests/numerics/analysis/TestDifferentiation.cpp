#include "../../Test.h"
#include <math/numerics/analysis/Differentiation.h>
#include <math/visualization/Plot.h>


class DifferentiationTestCase : public Test
{
    int m             = 10;
    double a          = 0;
    double b          = 2 * M_PI;
    Matrix<double> xk = linspace(a, b, m).Transpose();

    Matrix<double> f(const Matrix<double>& in) {
        return in.Apply([](const double& val) { return cos(val); });
    }

    Matrix<double> df(const Matrix<double>& in) {
        return in.Apply([](const double& val) { return -sin(val); });
    }

    bool TestDiffs() {
        auto expected = df(xk);
        auto f_xk     = f(xk);
        Matrix<double> backward({
        { 0 },
        { -0.84854514524284763 },
        { -0.9649299364248537 },
        { -0.6298132863519682 },
        { 0 },
        { 0.62981328635196787 },
        { 0.9649299364248537 },
        { 0.84854514524284785 },
        { 0.33511665007288594 },
        { 0 },
        });
        Matrix<double> forward({
        { -0.33511665007288566 },
        { -0.84854514524284763 },
        { -0.9649299364248537 },
        { -0.6298132863519682 },
        { 0 },
        { 0.62981328635196787 },
        { 0.9649299364248537 },
        { 0.84854514524284785 },
        { 0.33511665007288594 },
        { 0 },
        });
        Matrix<double> central({
        { 0 },
        { -0.90673754083385083 },
        { -0.79737161138841095 },
        { -0.31490664317598421 },
        { 0.31490664317598377 },
        { 0.79737161138841095 },
        { 0.90673754083385083 },
        { 0.59183089765786689 },
        { 0 },
        { 0 },
        });
        Matrix<double> backward2({
        { 0 },
        { 0 },
        { -0.46225496131552568 },
        { 0.31490664317598388 },
        { 0.94471992952795147 },
        { 1.132488261461297 },
        { 0.79035274965184488 },
        { 0.078402402487904935 },
        { 0 },
        { 0 },
        });
        Matrix<double> central4({
        { 0 },
        { 0 },
        { -0.38092014495112875 },
        { 0.45883090351816203 },
        { 1.0838898728938249 },
        { 1.201784724648338 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        });

        AssertEqual(backwardDiff(xk, f_xk), backward);
        AssertEqual(forwardDiff(xk, f_xk), forward);
        AssertEqual(centralDiff(xk, f_xk), central);
        AssertEqual(backwardDiff2(xk, f_xk), backward2);
        AssertEqual(centralDiff4(xk, f_xk), central4);

        Plot plot("diffs");
        plot.AddData(HorizontalConcat(xk, expected), "Analytical result");
        plot.AddData(HorizontalConcat(xk, forwardDiff(xk, f(xk))), "forward", DataTypes::DOTS);
        plot.AddData(HorizontalConcat(xk, backwardDiff(xk, f(xk))), "backward", DataTypes::DOTS);
        plot.AddData(HorizontalConcat(xk, centralDiff(xk, f(xk))), "centralDiff", DataTypes::DOTS);
        plot.AddData(HorizontalConcat(xk, backwardDiff2(xk, f(xk))), "backward2", DataTypes::DOTS);
        plot.AddData(HorizontalConcat(xk, centralDiff4(xk, f(xk))), "centralDiff4", DataTypes::DOTS);
        plot();

        return true;
    }

public:
    virtual void run() { TestDiffs(); }
};


int main() {
    DifferentiationTestCase().run();
    return 0;
}