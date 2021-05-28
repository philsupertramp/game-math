#include "Symbolic.h"

std::regex GetRegex(MathNodeType type)
{
    static const std::regex symbol_regex("([-]?[A-Za-z_]{1}[A-Za-z0-9_]*)");
    static const std::regex number_regex("([-]?[0-9]+)");
    static const std::regex parentheses_regex("\\(([\\(\\)+\\-*\\/\\^A-Za-z\\s0-9]*)\\)");
    static const std::regex operator_regex("([\\-+*/\\^]+)");
    static const std::regex any_regex("([\\x00-\\x7F]+)");
    switch(type) {
        case NodeType_Symbolic: return symbol_regex;
        case NodeType_Numeric: return number_regex;
        case NodeType_Parentheses: return parentheses_regex;
        case NodeType_Any: return any_regex;
        default: return operator_regex;
    }
}

std::shared_ptr<Operator> GenerateOperator(OperatorType type){

    switch(type) {
        case TYPE_ADDITION:
            return std::make_shared<Operator>(
            "+", [](double a, double b) { return a + b; }, OperatorPriority::OPClassLine);
        case TYPE_SUBTRACTION:
            return std::make_shared<Operator>(
            "-", [](double a, double b) { return a - b; }, OperatorPriority::OPClassLine);
        case TYPE_MULTIPLICATION:
            return std::make_shared<Operator>(
            "*", [](double a, double b) { return a * b; }, OperatorPriority::OPClassDot);
        case TYPE_DIVISION:
            return std::make_shared<Operator>(
            "/", [](double a, double b) { return a / b; }, OperatorPriority::OPClassDot);
        case TYPE_POWER:
            return std::make_shared<Operator>(
            "^", [](double a, double b) { return pow(a, b); }, OperatorPriority::OPClassDot);
        case TYPE_PARENTHESES_OPEN:
            return std::make_shared<Operator>(
            "(",
            []([[maybe_unused]] double a, [[maybe_unused]] double b) { return 0.0; },
            OperatorPriority::OPClassParentheses);
        case TYPE_PARENTHESES_CLOSE:
            return std::make_shared<Operator>(
            ")",
            []([[maybe_unused]] double a, [[maybe_unused]] double b) { return 0.0; },
            OperatorPriority::OPClassParentheses);
    }
    return std::make_shared<Operator>(
    "",
    []([[maybe_unused]] double a, [[maybe_unused]] double b) {
        std::cerr << "Operator not found.\n" << std::flush;
        return -1;
    },
    OperatorPriority::OPClassUnknown);
}

