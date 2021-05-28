#include "../../Test.h"
#include <math/numerics/analysis/NaturalSpline.h>
#include <math/visualization/Plot.h>


class NaturalSplineTestCase : public Test
{
    bool TestNaturalSpline(){
        auto tschebyscheff = [](double start, double end, const Matrix<double>& xi){
          return xi.Apply([start, end](double x){return (((end-start) * (x+1))/2)+start;});
        };
        auto runge = [](const Matrix<double>& in){
          return in.Apply([](double a){return 1 / (1 + a * a);});
        };
        auto realX = linspace(-5, 5, 9);
        auto xi    = tschebyscheff(-5, 5, realX).Transpose();
        auto yi    = runge(xi);
        auto x     = linspace(min(xi), max(xi), 33).Transpose();
        NaturalSpline spline(xi, yi);
        auto out = spline(x);
        Matrix<double> expected = {
        {
        0.0015974440894568689,
        -0.0048587999838341578,
        -0.0086086531838560389,
        -0.0069457246373396259,
        0.0028363765289842191,
        0.021147738640901272,
        0.036817073700078466,
        0.036376791160699383,
        0.0063593004769475492,
        -0.055706183380461391,
        -0.10830322337468343,
        -0.098918576952342463,
        0.024960998439937543,
        0.2890410353434083,
        0.61179622625082297,
        0.88489355364281053,
        1,
        0.88489355364281053,
        0.61179622625082297,
        0.2890410353434083,
        0.024960998439937543,
        -0.098918576952342463,
        -0.10830322337468343,
        -0.055706183380461391,
        0.0063593004769475492,
        0.036376791160699362,
        0.036817073700078466,
        0.021147738640901272,
        0.0028363765289842226,
        -0.0069457246373396328,
        -0.0086086531838560406,
        -0.0048587999838341587,
        0,
        }
        };

        out = out.Transpose();
        AssertEqual(expected, out);

        return true;
    }
public:
    virtual void run() override {
        TestNaturalSpline();
    }
};

int main() {
    NaturalSplineTestCase().run();
    return 0;
}