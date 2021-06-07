#pragma once


#include "MathNode.h"
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
/**
 * Representation of operand
 * can be numerical/symbolical
 */
class Operand : public MathNode
{
public:
    /**
     * default constructor
     * @param name
     */
    explicit Operand(const std::string& name)
    : MathNode(name) {
        connectionType = NodeConnectionType::ConnectionType_None;
    }
};

/**
 * Symbolic representation as node
 * holds a variable symbol.
 * Set `evaluationValue` prior to calling `Evaluate` to compute desired value
 */
class Symbolic : public Operand
{
public:
    //! value which replaces symbol during evaluation
    double evaluationValue = 1.0;

    /**
     * default constructor
     * @param name
     */
    explicit Symbolic(const std::string& name, double defaultValue = 0.0)
    : Operand(name)
    , evaluationValue(defaultValue) {
        type = MathNodeType::NodeType_Symbolic;
    }

    /**
     * copy constructor
     * @param other
     */
    Symbolic(const Symbolic& other)
    : Operand(other) {
        type            = other.type;
        evaluationValue = other.evaluationValue;
    }

    /**
     * returns given evaluation value
     * @return
     */
    [[nodiscard]] double Evaluate() const override { return isNegative ? evaluationValue * (-1.0) : evaluationValue; }
};

static std::map<std::string, std::shared_ptr<Symbolic>> DefaultSymbols = {
{ "pi",
       std::make_shared<Symbolic>("pi", 3.1415926535897932384626433832795028841971693993751058209749445923078164) },
{ "e", std::make_shared<Symbolic>("e", 2.71828182845904523536028747135266249775724709369995957496696762772407663) },
};

bool isConstant(const std::string& in);
bool hasSymbol(const std::vector<std::shared_ptr<Symbolic>>& container, const std::shared_ptr<Symbolic>& sym);

/**
 * Representation of numerical value
 */
class Number : public Operand
{
    //! real numerical value
    double numericValue;

public:
    /**
     * default constructor
     * @param val
     */
    explicit Number(const std::string& val)
    : Operand(val) {
        type         = MathNodeType::NodeType_Numeric;
        numericValue = strtod(value, nullptr);
    }

    /**
     * returns real value
     * @return if isNegative -numericValue else numericValue
     */
    [[nodiscard]] double Evaluate() const override { return isNegative ? numericValue * (-1.0) : numericValue; }
};
