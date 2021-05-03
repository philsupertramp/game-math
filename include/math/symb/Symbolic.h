#pragma once


#include <regex>
#include <sstream>
#include <utility>


enum MathNodeType {
    NodeType_Operator    = 0,
    NodeType_Symbolic    = 1,
    NodeType_Numeric     = 2,
    NodeType_Parentheses = 3,
    NodeType_Any         = 4,
};


std::regex GetRegex(MathNodeType type) {
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

/**
 * Base node type of Abstract syntax tree implementation
 */
class MathNode
{
protected:
    size_t valSize = 0;

public:
    bool isNegative     = false;
    bool hasParentheses = false;

    MathNodeType type;
    std::shared_ptr<MathNode> left  = nullptr;
    std::shared_ptr<MathNode> right = nullptr;
    char* value{};

    explicit MathNode(const std::string& val) {
        isNegative = val.find('-') != std::string::npos && val.size() > 1;
        value      = new char[val.size() + 1];
        std::copy(val.begin(), val.end(), value);
        value[val.size()] = '\0'; // don't forget the terminating 0
        valSize           = val.size() + 1;
    }
    MathNode(const MathNode& other) {
        valSize        = other.valSize;
        isNegative     = other.isNegative;
        hasParentheses = other.hasParentheses;
        value          = (char*)malloc(other.valSize);
        strcpy(value, other.value);
        left  = other.left;
        right = other.right;
        type  = other.type;
    }

    [[nodiscard]] virtual double Evaluate() const = 0;

    [[nodiscard]] std::string GetString() const {
        std::stringstream out;
        if(left != nullptr) out << left;
        out << value;
        if(right != nullptr) out << right;
        return out.str();
    }

    friend std::ostream& operator<<(std::ostream& ostr, const MathNode& other) {
        if(other.hasParentheses) ostr << "(";
        if(other.left != nullptr) ostr << other.left;
        ostr << (other.isNegative ? "-" : "") << other.value;
        if(other.right != nullptr) ostr << other.right;
        if(other.hasParentheses) ostr << ")";
        return ostr;
    }
    friend std::ostream& operator<<(std::ostream& ostr, MathNode* other) {
        if(other->hasParentheses) ostr << "(";
        if(other->left != nullptr) ostr << other->left;
        ostr << (other->isNegative ? "-" : "") << other->value;
        if(other->right != nullptr) ostr << other->right;
        if(other->hasParentheses) ostr << ")";
        return ostr;
    }
};

enum OperatorPriority {
    OPClassUnknown     = 0,
    OPClassLine        = 1,
    OPClassDot         = 2,
    OPClassParentheses = 3,
};
class Operator : public MathNode
{
public:
    std::function<double(double, double)> op;

    OperatorPriority priority;

    Operator(const std::string& name, std::function<double(double, double)> fun, OperatorPriority operatorPriority)
        : MathNode(name)
        , op(std::move(fun))
        , priority(operatorPriority) {
        type = MathNodeType::NodeType_Operator;
    }

    Operator(const Operator& other)
        : MathNode(other) {
        op       = other.op;
        priority = other.priority;
    }

    [[nodiscard]] double Evaluate() const override {
        assert(left != nullptr && right != nullptr);
        return op(left->Evaluate(), right->Evaluate());
    }
};

enum OperatorType {
    TYPE_ADDITION          = 0,
    TYPE_SUBTRACTION       = 1,
    TYPE_MULTIPLICATION    = 2,
    TYPE_DIVISION          = 3,
    TYPE_POWER             = 4,
    TYPE_PARENTHESES_OPEN  = 5,
    TYPE_PARENTHESES_CLOSE = 6,
};

enum OperatorValue : char {
    VALUE_ADDITION          = '+',
    VALUE_SUBTRACTION       = '-',
    VALUE_MULTIPLICATION    = '*',
    VALUE_DIVISION          = '/',
    VALUE_POWER             = '^',
    VALUE_PARENTHESES_OPEN  = '(',
    VALUE_PARENTHESES_CLOSE = ')',
};

std::shared_ptr<Operator> GenerateOperator(OperatorType type) {
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

class Operand : public MathNode
{
public:
    explicit Operand(const std::string& name)
        : MathNode(name) { }
};
class Symbolic : public Operand
{
public:
    double evaluationValue = 1.0;

    explicit Symbolic(const std::string& name)
        : Operand(name) {
        type = MathNodeType::NodeType_Symbolic;
    }

    Symbolic(const Symbolic& other)
        : Operand(other) { }

    [[nodiscard]] double Evaluate() const override { return isNegative ? evaluationValue * (-1.0) : evaluationValue; }
};

class Number : public Operand
{
    double numericValue;

public:
    explicit Number(const std::string& val)
        : Operand(val) {
        type         = MathNodeType::NodeType_Numeric;
        numericValue = strtod(value, nullptr);
    }

    [[nodiscard]] double Evaluate() const override { return isNegative ? numericValue * (-1.0) : numericValue; }
};

/**
 * Representation of mathematical statement
 *
 * \example
 * \code{.cpp}
 * Equation("x + 1");
 * Equation("x + (1 - y)");
 * Equation("x + (1 - y)^2");
 * ...
 * \endcode
 *
 */
class Equation
{
private:
    std::vector<std::shared_ptr<Symbolic>> symbols;

public:
    std::shared_ptr<MathNode> baseNode = nullptr;

    explicit Equation(const char* val) {
        std::string eq(val);
        baseNode = createAST(eq);
    }
    explicit Equation(const std::string& val) { baseNode = createAST(val); }

    void Print() {
        std::cout << baseNode;
        std::cout << std::endl;
    }

    /**
     * var arg method to evaluate equation using variable amount of symbols,
     * yet if called with parameters, all parameters need to be present.
     *
     * **Needs to be called with #symbols**
     *
     * @tparam VArgs variable amount of arguments
     * @param args arguments passed for symbols, with index representing index of symbol in symbols
     * @return Evaluation of equation
     *
     *
     *  \example
     *  \code{.cpp}
     *  Equation eq("x * 2 + y");
     *  // requires 2 arguments to be passed, first value for `x`, second for `y`
     *  assert(eq.Evaluate(1, 2) == 4);
     * \endcode
     */
    template<typename... VArgs>
    double Evaluate(VArgs... args) {
        assert(sizeof...(args) == symbols.size());
        SetSymbols(0, args...);
        return baseNode->Evaluate();
    }
    double Evaluate() { return baseNode->Evaluate(); }

    /**
     * var arg method to recursively set current values of symbols
     * @tparam VArgs variable amount of arguments
     * @param index current index of symbol
     * @param val value for symbol with index `index`
     * @param args following arguments
     */
    template<typename... VArgs>
    void SetSymbols(const int& index, double val, VArgs... args) {
        symbols[index]->evaluationValue = val;
        SetSymbols(index + 1, args...);
    }
    /**
     * empty method to stop va arg recursion
     * @param index
     */
    void SetSymbols([[maybe_unused]] const int& index) { }

    /**
     * Helper to check for existence of symbolic
     * @param sym symbol to search for
     * @return sym already registered?
     */
    bool HasSymbol(const std::shared_ptr<Symbolic>& sym) {
        for(const auto& elem : symbols) {
            if(elem == sym) return true;
        }
        return false;
    }

    [[nodiscard]] std::string GetString() const { return baseNode->GetString(); }

    /**
     * Helper method to display ASTree.
     */
    void PrintTree() const {
        int level    = 0;
        size_t depth = 0;
        std::vector<std::string> tree(GetDepth(baseNode, depth));
        PrintTree(baseNode, level, tree);

        for(const auto& line : tree) { std::cout << line << std::endl; }
    }

    /**
     * Getter for max AST depth
     * @param node to check depth of
     * @param current_depth counter for current depth level
     * @return current depth if last node in branch, else max of left depth and right depth
     */
    size_t GetDepth(const std::shared_ptr<MathNode>& node, size_t& current_depth) const {
        current_depth++;
        auto leftDep  = std::numeric_limits<size_t>::min();
        auto rightDep = std::numeric_limits<size_t>::min();
        if(node->left) { leftDep = GetDepth(node->left, current_depth); }
        if(node->right) { rightDep = GetDepth(node->right, current_depth); }

        return std::max(std::max(leftDep, current_depth), rightDep);
    }

    /**
     * Add representation string into tree of passed node
     * @param node the current node to process
     * @param level current depth level
     * @param tree tree representation as array of lines, each line represents a depth level
     */
    void PrintTree(const std::shared_ptr<MathNode>& node, int& level, std::vector<std::string>& tree) const {
        tree[level] += "\t";
        tree[level] += std::string(node->value);
        if(node->left) {
            PrintTree(node->left, level, tree);
            tree[level] += "\t";
            PrintTree(node->right, level, tree);
            tree[level] += "\t";
            level += 1;
        }
    }

private:
    /**
     * Determines first connecting operator
     * @param valString
     * @return
     */
    static std::shared_ptr<Operator> GetOperator(const std::string& valString) {
        if(valString.empty()) { return {}; }
        switch(int(strip(valString).c_str()[0])) {
            case int(OperatorValue::VALUE_MULTIPLICATION): return GenerateOperator(OperatorType::TYPE_MULTIPLICATION);
            case int(OperatorValue::VALUE_DIVISION): return GenerateOperator(OperatorType::TYPE_DIVISION);
            case int(OperatorValue::VALUE_ADDITION): return GenerateOperator(OperatorType::TYPE_ADDITION);
            case int(OperatorValue::VALUE_SUBTRACTION): return GenerateOperator(OperatorType::TYPE_SUBTRACTION);
            case int(OperatorValue::VALUE_POWER): return GenerateOperator(OperatorType::TYPE_POWER);
            case int(OperatorValue::VALUE_PARENTHESES_OPEN):
                return GenerateOperator(OperatorType::TYPE_PARENTHESES_OPEN);
            case int(OperatorValue::VALUE_PARENTHESES_CLOSE):
                return GenerateOperator(OperatorType::TYPE_PARENTHESES_CLOSE);
        }
        return {};
    }

    /**
     * Creates A(bstract)S(yntax)T(ree) using shunting-yard algorithm, thanks Mr. Dijkstra :)
     * https://en.wikipedia.org/wiki/Shunting-yard_algorithm
     * @param processString
     * @return
     */
    std::shared_ptr<MathNode> createAST(const std::string& processString) {
        auto regex_end = std::sregex_iterator();
        std::vector<std::string> operatorStack;
        std::vector<std::shared_ptr<MathNode>> operandStack;

        auto numberRegex    = GetRegex(MathNodeType::NodeType_Numeric);
        const auto isNumber = [numberRegex](const std::string& in) {
            std::cmatch m;
            return std::regex_match(in.c_str(), m, numberRegex);
        };
        auto symbolRegex    = GetRegex(MathNodeType::NodeType_Symbolic);
        const auto isSymbol = [symbolRegex](const std::string& in) {
            std::cmatch m;
            return std::regex_match(in.c_str(), m, symbolRegex);
        };
        auto operatorRegex    = GetRegex(MathNodeType::NodeType_Operator);
        const auto isOperator = [operatorRegex](const std::string& in) {
            std::cmatch m;
            return std::regex_match(in.c_str(), m, operatorRegex);
        };
        auto anyRegex    = GetRegex(MathNodeType::NodeType_Any);
        const auto isAny = [anyRegex](const std::string& in) {
            std::cmatch m;
            return std::regex_match(in.c_str(), m, anyRegex);
        };
        const auto isParenthesesOpen  = [](const std::string& in) { return in.find('(') != std::string::npos; };
        const auto isParenthesesClose = [](const std::string& in) { return in.find(')') != std::string::npos; };

        bool prevWasOperator = true;
        bool nextIsNegative  = false;

        for(const auto& c : split(strip(processString))) {
            if(isNumber(c)) {
                auto sym        = std::make_shared<Number>(c);
                sym->isNegative = nextIsNegative;
                nextIsNegative  = false;
                operandStack.push_back(sym);
            } else if(isSymbol(c)) {
                auto symbol        = std::make_shared<Symbolic>(c);
                symbol->isNegative = nextIsNegative;
                nextIsNegative     = false;
                operandStack.push_back(symbol);
                if(!HasSymbol(symbol)) symbols.push_back(symbol);
            } else if(isParenthesesOpen(c)) {
                operatorStack.push_back(c);
            } else if(isOperator(c)) {
                auto currentOp = GetOperator(c);
                if(prevWasOperator && currentOp->value[0] == '-') {
                    nextIsNegative = true;
                } else {
                    if(!operatorStack.empty()) {
                        auto stackTop = GetOperator(operatorStack[operatorStack.size() - 1]);
                        while(stackTop != nullptr && stackTop->priority >= currentOp->priority
                              && stackTop->value[0] != '(') {
                            operatorStack.pop_back();

                            // inverse order, first right then left
                            stackTop->right = operandStack[operandStack.size() - 1];

                            operandStack.pop_back();

                            stackTop->left           = operandStack[operandStack.size() - 1];
                            stackTop->hasParentheses = true;

                            operandStack.pop_back();

                            operandStack.push_back(stackTop);
                            if(!operatorStack.empty()) {
                                stackTop = GetOperator(operatorStack[operatorStack.size() - 1]);
                            } else {
                                break;
                            }
                        }
                    }
                    operatorStack.push_back(c);
                }
            } else if(isParenthesesClose(c)) {
                auto stackTop = operatorStack[operatorStack.size() - 1];
                while(!isParenthesesOpen(stackTop)) {
                    auto currentOp = GetOperator(stackTop);

                    currentOp->right = operandStack[operandStack.size() - 1];
                    operandStack.pop_back();

                    currentOp->left = operandStack[operandStack.size() - 1];
                    operandStack.pop_back();

                    operandStack.push_back(currentOp);
                    if(operatorStack.size() > 1) {
                        operatorStack.pop_back();

                        stackTop = operatorStack[operatorStack.size() - 1];
                    } else
                        break;
                }
                operatorStack.pop_back();
            } else if(isAny(c)) {
                // ignore
            } else {
                // error
                std::cerr << "Error detecting character " << c << std::endl;
                return nullptr;
            }
            prevWasOperator = isOperator(c);
        }

        while(!operatorStack.empty()) {
            auto stackTop = operatorStack[operatorStack.size() - 1];
            operatorStack.pop_back();

            auto currentOp   = GetOperator(stackTop);
            currentOp->right = operandStack[operandStack.size() - 1];
            operandStack.pop_back();

            currentOp->left = operandStack[operandStack.size() - 1];
            operandStack.pop_back();

            operandStack.push_back(currentOp);
        }

        return operandStack.front();
    }
};
