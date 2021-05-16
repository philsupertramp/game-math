#include "../../Test.h"
#include <math/numerics/analysis/NaturalSpline.h>


class NaturalSplineTestCase : public Test
{
    bool TestNaturalSpline(){
        auto tschebyscheff = [](double start, double end, const Matrix<double>& xi){
            return xi.Apply([start, end](double x){return (((end-start) * (x+1))/2)+start;});
        };
        auto runge = [](const Matrix<double>& in){
            return in.Apply([](double a){return 1 / (1 + a * a);});
        };
        auto realX = linspace(-5, 5, 3);
        auto xi    = tschebyscheff(-5, 5, realX).Transpose();
        auto yi    = runge(xi);
        auto x     = linspace(min(xi), max(xi), 3).Transpose();
        NaturalSpline spline(xi, yi);
        auto out = spline(x);
        Matrix<double> expected = { { 0.0015974440894568689, 1., 0. } };
        expected = expected.Transpose();

        AssertEqual(expected, out);

        return true;
    }
public:
    virtual void run() override { TestNaturalSpline();
    }
};

int main() {
    NaturalSplineTestCase().run();
    return 0;
}