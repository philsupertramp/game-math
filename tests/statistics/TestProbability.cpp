#include "../Test.h"
#include <math/statistics/Probability.h>

class ProbabilityTestCase
: public Test
{
	bool TestLM(){
		Matrix<double> x = {{1585.,1819.,1647.,1496.,921.,1278.,1810.,1987.,1612.,1413.}};
		Matrix<double> y = {{12.5,10.,9.95,11.5,12.,10.,8.,9.,9.5,12.5}};
		x = x.Transpose();
		y = y.Transpose();

		auto out = lm(x, y);

		AssertEqual(out.beta_0, -0.00313362);
		AssertEqual(out.beta_1, 15.3734);

		return true;
	}
	public:
		virtual void run() override {
			TestLM();
		}
};

int main(){
	ProbabilityTestCase().run();
	return 0;
}
