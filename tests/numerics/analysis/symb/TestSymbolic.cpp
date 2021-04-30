#include "../../../Test.h"
#include <math/numerics/analysis/symb/Symbolic.h>


class SymbolicTestCase : public Test
{
    bool TestMathematicalProperties() {
        Equation equation = Equation("2 + x + 1 + 2 + y");
        AssertEqual(equation.Evaluate(1, 2), 8);
        AssertEqual(equation.Evaluate(3, 3), 11);
        AssertEqual(equation.Evaluate(6, 4), 15);

        equation = Equation("x * 2");
        AssertEqual(equation.Evaluate(1), 2);
        AssertEqual(equation.Evaluate(3), 6);
        AssertEqual(equation.Evaluate(6), 12);

        equation = Equation("5 + x * 2");
        AssertEqual(equation.Evaluate(1), 7);
        AssertEqual(equation.Evaluate(3), 11);
        AssertEqual(equation.Evaluate(6), 17);

        Equation equation1("x * 2 + 5");
        for(int i = -10; i < 10; ++i) { AssertEqual(equation.Evaluate(i), equation1.Evaluate(i)); }
        equation  = Equation("5 * 3 + 2 * x");
        equation1 = Equation("5 * 3 + x * 2");
        Equation equation2("2 * x + 5 * 3");
        Equation equation3("x * 2 + 3 * 5");
        for(int i = -10; i < 10; ++i) {
            // Commutative property of multiplication
            AssertEqual(equation.Evaluate(i), equation1.Evaluate(i));
            // Commutative property of addition
            AssertEqual(equation1.Evaluate(i), equation2.Evaluate(i));
            // Commutative property of addition and multiplication
            AssertEqual(equation2.Evaluate(i), equation3.Evaluate(i));
        }

        equation = Equation("5 / x  - 2");
        AssertEqual(equation.Evaluate(1), 3);
        AssertEqual(equation.Evaluate(5), -1);
        AssertEqual(equation.Evaluate(2), 0.5);

        equation = Equation("x^2  - 5");
        AssertEqual(equation.Evaluate(1), -4);
        AssertEqual(equation.Evaluate(5), 20);
        AssertEqual(equation.Evaluate(2), -1);

        return true;
    }
    bool TestNegative() {
        Equation equation("-x + 5");
        AssertEqual(equation.Evaluate(2), 3);
        AssertEqual(equation.Evaluate(-2), 7);
        equation = Equation("-x + -5");
        AssertEqual(equation.Evaluate(2), -7);
        AssertEqual(equation.Evaluate(-2), -3);
        equation = Equation("-x - -5");
        AssertEqual(equation.Evaluate(2), 3);
        AssertEqual(equation.Evaluate(-2), 7);
        return true;
    }

public:
    void run() override {
        TestMathematicalProperties();
        TestNegative();
    }
};

int main() {
    SymbolicTestCase().run();
    return 0;
}