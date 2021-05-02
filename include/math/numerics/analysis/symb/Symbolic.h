#pragma once


#include <regex>
#include <sstream>
#include <utility>


enum MathNodeType {
    NodeType_Operator    = 0,
    NodeType_Symbolic    = 1,
    NodeType_Numeric     = 2,
    NodeType_Parentheses = 3,
};


std::regex GetRegex(MathNodeType type) {
    static const std::regex symbol_regex("([-]?[A-Za-z_]{1}[A-Za-z0-9_]*)");
    static const std::regex number_regex("([-]?[0-9]+)");
    static const std::regex parentheses_regex("\\(([\\(\\)+\\-*\\/\\^A-Za-z\\s0-9]*)\\)");
    static const std::regex operator_regex("([\\-+*/\\^]+)");
    switch(type) {
        case NodeType_Symbolic: return symbol_regex;
        case NodeType_Numeric: return number_regex;
        case NodeType_Parentheses: return parentheses_regex;
        default: return operator_regex;
    }
}

class MathNode
{
protected:
    size_t valSize = 0;

public:
    bool isNegative     = false;
    bool hasParentheses = false;

    MathNodeType type;
    MathNode* left  = nullptr;
    MathNode* right = nullptr;
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

enum OperatorClass {
    OPClassUnknown     = 0,
    OPClassLine        = 1,
    OPClassDot         = 2,
    OPClassParentheses = 3,
};
class Operator : public MathNode
{
public:
    std::function<double(double, double)> op;

    OperatorClass opClass;

    Operator(const std::string& name, std::function<double(double, double)> fun, OperatorClass operatorClass)
        : MathNode(name)
        , op(std::move(fun))
        , opClass(operatorClass) {
        type = MathNodeType::NodeType_Operator;
    }

    Operator(const Operator& other)
        : MathNode(other) {
        op = other.op;
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
    VALUE_ADDITION       = '+',
    VALUE_SUBTRACTION    = '-',
    VALUE_MULTIPLICATION = '*',
    VALUE_DIVISION       = '/',
    VALUE_POWER          = '^'
};

Operator* GenerateOperator(OperatorType type) {
    switch(type) {
        case TYPE_ADDITION:
            return new Operator(
            "+", [](double a, double b) { return a + b; }, OperatorClass::OPClassLine);
        case TYPE_SUBTRACTION:
            return new Operator(
            "-", [](double a, double b) { return a - b; }, OperatorClass::OPClassLine);
        case TYPE_MULTIPLICATION:
            return new Operator(
            "*", [](double a, double b) { return a * b; }, OperatorClass::OPClassDot);
        case TYPE_DIVISION:
            return new Operator(
            "/", [](double a, double b) { return a / b; }, OperatorClass::OPClassDot);
        case TYPE_POWER:
            return new Operator(
            "^", [](double a, double b) { return pow(a, b); }, OperatorClass::OPClassDot);
        case TYPE_PARENTHESES_OPEN:
            return new Operator(
            "(",
            []([[maybe_unused]] double a, [[maybe_unused]] double b) { return 0.0; },
            OperatorClass::OPClassParentheses);
        case TYPE_PARENTHESES_CLOSE:
            return new Operator(
            ")",
            []([[maybe_unused]] double a, [[maybe_unused]] double b) { return 0.0; },
            OperatorClass::OPClassParentheses);
    }
    return new Operator(
    "",
    []([[maybe_unused]] double a, [[maybe_unused]] double b) {
        std::cerr << "Operator not found.\n" << std::flush;
        return -1;
    },
    OperatorClass::OPClassUnknown);
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

class Equation
{
public:
    class EquationNode : public Operand
    {
    public:
        Equation* eq;

        explicit EquationNode(const std::string& val)
            : Operand(strip(val)) {
            type = MathNodeType::NodeType_Parentheses;
            eq   = new Equation(strip(val));
            eq->values.resize(eq->symbols.size());
        }
        [[nodiscard]] double Evaluate() const override {
            for(int i = 0; i < (int)eq->values.size(); ++i) { eq->SetSymbols(i, eq->values[i]); }
            return eq->Evaluate();
        }
    };

private:
    std::vector<double> values;
    MathNode* baseNode = nullptr;
    std::vector<Symbolic*> symbols;
    std::vector<EquationNode*> equations;
    bool hasParentheses = false;

public:
    explicit Equation(const char* val) {
        std::string eq(val);

        Generate(eq);
    }
    explicit Equation(const std::string& val) { Generate(val); }

    void Generate(const std::string& eq) { baseNode = createNode(eq); }

    void Print() {
        std::cout << baseNode;
        std::cout << std::endl;
    }
    void SetArg(int index, double value) { values[index] = value; }

    template<typename... Args>
    double Evaluate(Args... args) {
        assert(sizeof...(args) == symbols.size());
        SetSymbols(0, args...);
        return baseNode->Evaluate();
    }
    double Evaluate() { return baseNode->Evaluate(); }
    template<typename... Args>
    void SetSymbols(const int& index, double val, Args... args) {
        symbols[index]->evaluationValue = val;

        for(auto elem : equations) { elem->eq->SetArg(index, val); }
        SetSymbols(index + 1, args...);
    }
    void SetSymbols([[maybe_unused]] const int& index) { }

    bool HasSymbol(Symbolic* sym) {
        for(auto elem : symbols) {
            if(elem == sym) return true;
        }
        return false;
    }

    [[nodiscard]] std::string GetString() const { return baseNode->GetString(); }

    void PrintTree() const {
        int level    = 0;
        size_t depth = 0;
        std::vector<std::string> tree(GetDepth(baseNode, depth));
        PrintTree(baseNode, level, tree);

        for(const auto& line : tree) { std::cout << line << std::endl; }
    }

    size_t GetDepth(MathNode* node, size_t& current_depth) const {
        current_depth++;
        auto leftDep  = std::numeric_limits<size_t>::min();
        auto rightDep = std::numeric_limits<size_t>::min();

        if(node->left) { leftDep = GetDepth(node->left, current_depth); }

        if(node->right) { rightDep = GetDepth(node->right, current_depth); }

        return std::max(std::max(leftDep, current_depth), rightDep);
    }

    void PrintTree(MathNode* node, int& level, std::vector<std::string>& tree) const {
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
    template<typename... Args>
    [[nodiscard]] int GetMinValIndex(int a, int b, Args... args) const {
        auto aVal = a;
        if(aVal == -1) { aVal = std::numeric_limits<int>::max(); }
        auto bVal = b;
        if(bVal == -1) { bVal = std::numeric_limits<int>::max(); }
        auto minVal = std::min(aVal, bVal);
        return GetMinValIndex(minVal, args...);
    }
    [[nodiscard]] int GetMinValIndex(int a, int b) const {
        auto aVal = a;
        if(aVal == -1) { aVal = std::numeric_limits<int>::max(); }
        auto bVal = b;
        if(bVal == -1) { bVal = std::numeric_limits<int>::max(); }
        auto minVal = std::min(aVal, bVal);
        return minVal == std::numeric_limits<int>::max() ? -1 : minVal;
    }
    /**
     * Determines first connecting operator
     * @param valString
     * @return
     */
    Operator* GetOperator(const std::string& valString) {
        if(valString.empty()) { return {}; }
        switch(int(strip(valString).c_str()[0])) {
            case int('*'): return GenerateOperator(OperatorType::TYPE_MULTIPLICATION);
            case int('/'): return GenerateOperator(OperatorType::TYPE_DIVISION);
            case int('+'): return GenerateOperator(OperatorType::TYPE_ADDITION);
            case int('-'): return GenerateOperator(OperatorType::TYPE_SUBTRACTION);
            case int('^'): return GenerateOperator(OperatorType::TYPE_POWER);
            case int('('): return GenerateOperator(OperatorType::TYPE_PARENTHESES_OPEN);
            case int(')'): return GenerateOperator(OperatorType::TYPE_PARENTHESES_CLOSE);
        }
        return {};
    }

    MathNode* createNode(const std::string& processString) {
        auto regex_end = std::sregex_iterator();
        std::vector<std::string> operatorStack;
        std::vector<MathNode*> operandStack;

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
        const auto isParenthesesOpen  = [](const std::string& in) { return in.find('(') != std::string::npos; };
        const auto isParenthesesClose = [](const std::string& in) { return in.find(')') != std::string::npos; };

        bool prevWasOperator = true;
        bool nextIsNegative  = false;

        for(const auto& c : split(strip(processString))) {
            if(isNumber(c)) {
                auto sym        = new Number(c);
                sym->isNegative = nextIsNegative;
                nextIsNegative  = false;
                operandStack.push_back(sym);
            } else if(isSymbol(c)) {
                auto symbol        = new Symbolic(c);
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
                        while(stackTop != nullptr && stackTop->opClass >= currentOp->opClass
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
            } else {
                // error
                std::cerr << "Error detecting character " << c << std::endl;
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
    /*
     *

     */
};
