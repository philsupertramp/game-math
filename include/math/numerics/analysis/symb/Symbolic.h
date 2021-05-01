#pragma once


#include <regex>
#include <sstream>
#include <utility>


class MathNode
{
protected:
    size_t valSize  = 0;
    bool isNegative = false;

public:
    MathNode* left  = nullptr;
    MathNode* right = nullptr;
    char* value{};

    explicit MathNode(const std::string& val) {
        isNegative = val.find('-') != val.npos && val.size() > 1;
        value      = new char[val.size() + 1];
        std::copy(val.begin(), val.end(), value);
        value[val.size()] = '\0'; // don't forget the terminating 0
        valSize           = val.size() + 1;
    }
    MathNode(const MathNode& other) {
        valSize    = other.valSize;
        isNegative = other.isNegative;
        value      = (char*)malloc(other.valSize);
        strcpy(value, other.value);
        left  = other.left;
        right = other.right;
    }

    virtual double Evaluate() const = 0;

    friend std::ostream& operator<<(std::ostream& ostr, const MathNode& other) {
        if(other.left != nullptr) ostr << other.left;
        ostr << other.value;
        if(other.right != nullptr) ostr << other.right;
        return ostr;
    }
    friend std::ostream& operator<<(std::ostream& ostr, MathNode* other) {
        if(other->left != nullptr) ostr << other->left;
        ostr << other->value;
        if(other->right != nullptr) ostr << other->right;
        return ostr;
    }
};
class Symbolic : public MathNode
{
public:
    double evaluationValue = 1.0;

    Symbolic(const std::string& name)
        : MathNode(name) { }

    Symbolic(const Symbolic& other)
        : MathNode(other) { }

    double Evaluate() const override { return isNegative ? evaluationValue * (-1.0) : evaluationValue; }
};

class Operator : public MathNode
{
    std::function<double(double, double)> op;

public:
    Operator(const std::string& name, std::function<double(double, double)> fun)
        : MathNode(name)
        , op(std::move(fun)) { }

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
    TYPE_ADDITION       = 0,
    TYPE_SUBTRACTION    = 1,
    TYPE_MULTIPLICATION = 2,
    TYPE_DIVISION       = 3,
    TYPE_POWER          = 4
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
        case TYPE_ADDITION: return new Operator("+", [](double a, double b) { return a + b; });
        case TYPE_SUBTRACTION: return new Operator("-", [](double a, double b) { return a - b; });
        case TYPE_MULTIPLICATION: return new Operator("*", [](double a, double b) { return a * b; });
        case TYPE_DIVISION: return new Operator("/", [](double a, double b) { return a / b; });
        case TYPE_POWER: return new Operator("^", [](double a, double b) { return pow(a, b); });
    }
}

class Number : public MathNode
{
public:
    Number(const std::string& val)
        : MathNode(val) { }

    [[nodiscard]] double Evaluate() const override { return atof(value); }
};

enum MathNodeType {
    NodeType_Operator = 0,
    NodeType_Symbolic = 1,
    NodeType_Numeric  = 2,
};

const std::regex symbol_regex("([-]?[A-Za-z_]{1}[A-Za-z0-9_]*)");
const std::regex number_regex("([-]?[0-9]+)");
const std::regex operator_regex("([-+*/\\^]+)");

std::regex GetRegex(MathNodeType type) {
    switch(type) {
        case NodeType_Symbolic: return symbol_regex;
        case NodeType_Numeric: return number_regex;
        default: return operator_regex;
    }
}


class Equation
{
    MathNode* baseNode;
    std::vector<Symbolic*> symbols;

public:
    explicit Equation(const char* val) {
        std::string eq(val);

        Generate(eq);
    }
    /**
     * Idea:
     *
     *          x + 1
     *            | cut
     *            +
     *         /     \
     *        x      1
     * Ignores parentheses for now
     * @param eq string representation of equation
     */
    void Generate(const std::string& eq) {
        auto openParentheses = eq.find('(');
        bool hasParentheses  = openParentheses != eq.npos;
        MathNode* node;
        std::string right;

        if(hasParentheses) {
            // build parentheses-stack for left side
        } else {
            // build left side from beginning until first operator
            // allow early exit
            node = createNode(eq);
        }
        baseNode = node;
    }

    void Print() {
        std::cout << baseNode;
        std::cout << std::endl;
    }

    template<typename... Args>
    double Evaluate(Args... args) {
        assert(sizeof...(args) == symbols.size());
        SetSymbols(0, args...);

        return baseNode->Evaluate();
    }
    template<typename... Args>
    void SetSymbols(const int& index, double val, Args... args) {
        symbols[index]->evaluationValue = val;
        SetSymbols(index + 1, args...);
    }
    void SetSymbols([[maybe_unused]] const int& index) { }

    bool HasSymbol(Symbolic* sym) {
        for(auto elem : symbols) {
            if(elem == sym) return true;
        }
        return false;
    }

private:
    std::pair<size_t, Operator*> GetOperator(const std::string& valString) {
        size_t addStart = 0, subStart = 0, mulStart = 0, divStart = 0, powStart = 0;
        size_t retries = 0;
        while(retries < 3) {
            addStart = valString.find(OperatorValue::VALUE_ADDITION, addStart + 1);
            if(addStart != valString.npos && addStart != 0) {
                return { addStart, GenerateOperator(OperatorType::TYPE_ADDITION) };
            }
            subStart = valString.find(OperatorValue::VALUE_SUBTRACTION, subStart + 1);
            if(subStart != valString.npos && subStart != 0) {
                return { subStart, GenerateOperator(OperatorType::TYPE_SUBTRACTION) };
            }
            mulStart = valString.find(OperatorValue::VALUE_MULTIPLICATION, mulStart + 1);
            if(mulStart != valString.npos && subStart != 0) {
                return { mulStart, GenerateOperator(OperatorType::TYPE_MULTIPLICATION) };
            }
            divStart = valString.find(OperatorValue::VALUE_DIVISION, divStart + 1);
            if(divStart != valString.npos && divStart != 0) {
                return { divStart, GenerateOperator(OperatorType::TYPE_DIVISION) };
            }
            powStart = valString.find(OperatorValue::VALUE_POWER, powStart + 1);
            if(powStart != valString.npos && powStart != 0) {
                return { powStart, GenerateOperator(OperatorType::TYPE_POWER) };
            }
            retries++;
        }
        return {};
    }
    MathNode* createNode(const std::string& valString) {
        auto operatorRes = GetOperator(valString);
        MathNode* node   = operatorRes.second;
        auto opStart     = operatorRes.first;
        if(opStart != valString.npos) {
            auto leftSide  = valString.substr(0, opStart);
            auto rightSide = valString.substr(opStart + 1, valString.size() - opStart);
            if(!appendSide(node, leftSide, true)) {
                // append not happening, string contains multiple values
                node->left = createNode(leftSide);
            }
            if(!appendSide(node, right Side, false)) { node->right = createNode(rightSide); }
        }
        return node;
    }
    bool appendSide(MathNode* node, const std::string& valString, bool appendLeft) {
        auto regex_end      = std::sregex_iterator();
        bool hasSingleValue = false;
        {
            auto num_begin  = std::sregex_iterator(valString.begin(), valString.end(), number_regex);
            auto numMatches = std::distance(num_begin, regex_end);
            if(numMatches > 1) {
                //error
                return false;
            }
            for(std::sregex_iterator i = num_begin; i != regex_end; ++i) {
                std::smatch match = *i;
                auto symbol    = new Number(match.str());
                if(appendLeft) {
                    node->left    = symbol;
                }
                else {
                    node->right = symbol;
                } 
            }
            hasSingleValue = numMatches > 0;
        }
        {
            auto symbol_begin = std::sregex_iterator(valString.begin(), valString.end(), symbol_regex);

            auto symbolCount = std::distance(symbol_begin, regex_end);
            if(symbolCount > 1 || (symbolCount > 0 && hasSingleValue)) { return false; }
            for(std::sregex_iterator i = symbol_begin; i != regex_end; ++i) {
                std::smatch match = *i;
                auto symbol       = new Symbolic(match.str());
                if(appendLeft) {
                    node->left    = symbol;
                }
                else {
                    node->right = symbol;
                } 
                if(!HasSymbol(symbol)) symbols.push_back(symbol);
            }
            hasSingleValue = hasSingleValue || symbolCount > 0;
        }
        return hasSingleValue;
    }
};
