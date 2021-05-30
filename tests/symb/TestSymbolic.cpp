#include "../Test.h"
#include <math/symb/Symbolic.h>


class SymbolicTestCase : public Test
{
    friend Equation;

    bool TestMathematicalProperties() {
        Equation equation = Equation("2 + x + 1 + 2 + y");
        AssertEqual(equation(1, 2), 8);
        AssertEqual(equation(3, 3), 11);
        AssertEqual(equation(6, 4), 15);

        equation = Equation("x * 2");
        AssertEqual(equation(1), 2);
        AssertEqual(equation(3), 6);
        AssertEqual(equation(6), 12);

        equation = Equation("5 + x * 2");
        AssertEqual(equation(1), 7);
        AssertEqual(equation(3), 11);
        AssertEqual(equation(6), 17);

        Equation equation1("x * 2 + 5");
        for(int i = -10; i < 10; ++i) { AssertEqual(equation(i), equation1(i)); }
        equation  = Equation("5 * 3 + 2 * x");
        equation1 = Equation("5 * 3 + x * 2");
        Equation equation2("2 * x + 5 * 3");
        Equation equation3("x * 2 + 3 * 5");
        for(int i = -10; i < 10; ++i) {
            // Commutative property of multiplication
            AssertEqual(equation(i), equation1(i));
            // Commutative property of addition
            AssertEqual(equation1(i), equation2(i));
            // Commutative property of addition and multiplication
            AssertEqual(equation2(i), equation3(i));
        }

        equation = Equation("5 / x  - 2");
        AssertEqual(equation(1), 3);
        AssertEqual(equation(5), -1);
        AssertEqual(equation(2), 0.5);

        equation = Equation("x^2  - 5");
        AssertEqual(equation(1), -4);
        AssertEqual(equation(5), 20);
        AssertEqual(equation(2), -1);

        return true;
    }
    bool TestNegative() {
        Equation equation("-x + 5");
        AssertEqual(equation(2), 3);
        AssertEqual(equation(-2), 7);
        equation = Equation("-x + -5");
        AssertEqual(equation(2), -7);
        AssertEqual(equation(-2), -3);
        equation = Equation("-x - -5");
        AssertEqual(equation(2), 3);
        AssertEqual(equation(-2), 7);
        return true;
    }

    bool TestParentheses() {
        Equation equation("2 * (x - 1)");
        AssertEqual(equation(1), 0);
        AssertEqual(equation(2), 2);
        AssertEqual(equation(60), 118);
        equation = Equation("2 * (x - (1 * y)) - 2");

        equation.Print();
        AssertEqual(equation(1, 1), -2);
        AssertEqual(equation(1, 2), -4);

        equation = Equation("2 * (x - 1 + y) - 2");

        equation.Print();
        AssertEqual(equation(1, 1), 0);
        AssertEqual(equation(1, 2), 2);

        equation = Equation("(x - 1 + y) * 2 - 2");

        equation.Print();
        AssertEqual(equation(1, 1), 0);
        AssertEqual(equation(1, 2), 2);

        equation = Equation("2 * (x - 1) - 2");

        equation.Print();
        equation.PrintTree();
        AssertEqual(equation(1), -2);
        AssertEqual(equation(2), 0);
        AssertEqual(equation(3), 2);
        AssertEqual(equation(99), 194);
        return true;
    }

    bool TestFailsGracefully() {
        Equation eq("µ");
        AssertTrue(eq.baseNode == nullptr);
        return true;
    }

    bool TestGetRegex() {
        auto sym = GetRegex(MathNodeType::NodeType_Symbolic);
        auto num = GetRegex(MathNodeType::NodeType_Numeric);
        auto ope = GetRegex(MathNodeType::NodeType_Operator);
        auto par = GetRegex(MathNodeType::NodeType_Parentheses);
        auto any = GetRegex(MathNodeType::NodeType_Any);

        std::cmatch m;
        AssertTrue(std::regex_match("x", m, sym));
        AssertFalse(std::regex_match("+", m, sym));

        AssertTrue(std::regex_match("2", m, num));
        AssertFalse(std::regex_match("x", m, num));

        AssertTrue(std::regex_match("+", m, ope));
        AssertFalse(std::regex_match("2", m, ope));

        AssertTrue(std::regex_match("(1)", m, par));
        AssertFalse(std::regex_match("1", m, par));

        AssertTrue(std::regex_match("^", m, any));
        AssertFalse(std::regex_match("µ", m, any));

        return true;
    }
    bool TestGenerateOperator() {
        const std::shared_ptr<Operator>& pow       = GenerateOperator(OperatorType::TYPE_POWER);
        const std::shared_ptr<Operator>& add       = GenerateOperator(OperatorType::TYPE_ADDITION);
        const std::shared_ptr<Operator>& sub       = GenerateOperator(OperatorType::TYPE_SUBTRACTION);
        const std::shared_ptr<Operator>& mul       = GenerateOperator(OperatorType::TYPE_MULTIPLICATION);
        const std::shared_ptr<Operator>& div       = GenerateOperator(OperatorType::TYPE_DIVISION);
        const std::shared_ptr<Operator>& par_open  = GenerateOperator(OperatorType::TYPE_PARENTHESES_OPEN);
        const std::shared_ptr<Operator>& par_close = GenerateOperator(OperatorType::TYPE_PARENTHESES_CLOSE);
        const std::shared_ptr<Operator>& empty     = GenerateOperator(static_cast<OperatorType>(-1));

        AssertEqual(pow->value, "^");
        AssertEqual(pow->op(1, 2), 1);

        AssertEqual(add->value, "+");
        AssertEqual(add->op(1, 2), 3);

        AssertEqual(sub->value, "-");
        AssertEqual(sub->op(1, 2), -1);

        AssertEqual(mul->value, "*");
        AssertEqual(mul->op(1, 2), 2);

        AssertEqual(div->value, "/");
        AssertEqual(div->op(1, 2), 0.5);

        AssertEqual(par_open->value, "(");
        AssertEqual(par_open->op(1, 2), 0.0);

        AssertEqual(par_close->value, ")");
        AssertEqual(par_close->op(1, 2), 0.0);

        AssertEqual(empty->value, "");
        AssertEqual(empty->op(1, 2), -1);

        return true;
    }

public:
    void run() override {
        TestMathematicalProperties();
        TestNegative();
        TestParentheses();
        TestFailsGracefully();
        TestGetRegex();
        TestGenerateOperator();
    }
};

int main() {
    SymbolicTestCase().run();
    return 0;
}