#pragma once


#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_set>
#include <utility>

#include "../format.h"


/**
 * Node type representation
 */
enum MathNodeType {
    NodeType_Operator      = 0,
    NodeType_Symbolic      = 1,
    NodeType_Numeric       = 2,
    NodeType_Parentheses   = 3,
    NodeType_Functional    = 4,
    NodeType_DefaultSymbol = 5,
    NodeType_Any           = 6,
};

enum NodeConnectionType {
    ConnectionType_Dual    = 0,
    ConnectionType_Left    = 2,
    ConnectionType_Right   = 3,
    ConnectionType_None    = 4,
    ConnectionType_Unknown = 5,
};

/**
 * Priority definition for AST generation, used in shunting-yard algorithm,
 * the Equation class builds nodes using a stack based on priorities
 */
enum OperatorPriority {
    OPClassUnknown     = 0,
    OPClassLine        = 1,
    OPClassDot         = 2,
    OPClassParentheses = 3,
    OPClassFunction    = 4,
};

/**
 * Representation of operator type used in factories
 */
enum OperatorType {
    TYPE_ADDITION          = 0, // ConnectionType_Dual
    TYPE_SUBTRACTION       = 1, // ConnectionType_Dual
    TYPE_MULTIPLICATION    = 2, // ConnectionType_Dual
    TYPE_DIVISION          = 3, // ConnectionType_Dual
    TYPE_POWER             = 4, // ConnectionType_Dual
    TYPE_PARENTHESES_OPEN  = 5, // ConnectionType_None
    TYPE_PARENTHESES_CLOSE = 6, // ConnectionType_None
};

/**
 * representation of operator value in char
 */
enum OperatorValue : char {
    //! l + r
    VALUE_ADDITION          = '+',
    //! l - r
    VALUE_SUBTRACTION       = '-',
    //! l * r
    VALUE_MULTIPLICATION    = '*',
    // l / r
    VALUE_DIVISION          = '/',
    //! l^r
    VALUE_POWER             = '^',
    //! (
    VALUE_PARENTHESES_OPEN  = '(',
    //! )
    VALUE_PARENTHESES_CLOSE = ')',
};

/**
 * regex factory for node types
 * @param type
 * @return regex for given type
 */
std::regex GetRegex(MathNodeType type);

/**
 * Base node type of Abstract syntax tree implementation
 */
class MathNode
{
public:
    //! helper to determine whether a node is negated or not
    bool isNegative     = false;
    //! helper to determine if node is within parentheses
    bool hasParentheses = false;
    //! node type representation
    MathNodeType type;
    //! type of connection allowed for this node
    NodeConnectionType connectionType = NodeConnectionType::ConnectionType_Dual;
    //! next node on left side
    std::shared_ptr<MathNode> left  = nullptr;
    //! next node on right side
    std::shared_ptr<MathNode> right = nullptr;
    //! char representation of value
    char* value{};
protected:
    //! helper to store size of value
    size_t valSize = 0;

public:
    /**
     * Default constructor
     * @param val
     */
    explicit MathNode(const std::string& val) {
        auto realValue = strip(val); // strip to allow comparison
        isNegative     = realValue.find('-') != std::string::npos && realValue.size() > 1;
        value          = new char[realValue.size() + 1];
        std::copy(realValue.begin(), realValue.end(), value);
        value[realValue.size()] = '\0'; // don't forget the terminating 0
        valSize                 = realValue.size() + 1;
    }

    /**
     * Copy constructor
     * @param other
     */
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

    /**
     * Interface function for evaluation of node,
     * needs to be implemented to add new type of node
     * @return
     */
    [[nodiscard]] virtual double Evaluate() const = 0;

    /**
     * Getter for string representation of node
     * @return
     */
    [[nodiscard]] std::string GetString() const {
        std::stringstream out;
        if(left != nullptr) out << left;
        out << value;
        if(right != nullptr) out << right;
        return out.str();
    }

    friend bool operator==(const MathNode& lhs, const MathNode& rhs) {
        return strcmp(lhs.value, rhs.value) == 0 && lhs.isNegative == rhs.isNegative;
    }

    bool operator==(MathNode* rhs) { return strcmp(value, rhs->value) == 0 && isNegative == rhs->isNegative; }

    /**
     * beautified output operator
     * @param ostr
     * @param other
     * @return
     */
    friend std::ostream& operator<<(std::ostream& ostr, const MathNode& other) {
        if(other.hasParentheses) ostr << "(";
        if(other.left != nullptr) ostr << other.left;
        ostr << (other.isNegative ? "-" : "") << other.value;
        if(other.right != nullptr) ostr << other.right;
        if(other.hasParentheses) ostr << ")";
        return ostr;
    }
    /**
     * beautified output operator
     * @param ostr
     * @param other
     * @return
     */
    friend std::ostream& operator<<(std::ostream& ostr, MathNode* other) {
        // early exit
        if(other == nullptr) return ostr;

        if(other->hasParentheses) ostr << "(";
        if(other->left != nullptr) ostr << other->left;
        ostr << (other->isNegative ? "-" : "") << other->value;
        if(other->right != nullptr) ostr << other->right;
        if(other->hasParentheses) ostr << ")";
        return ostr;
    }
};

/**
 * Left-to-right operator representation
 */
class Operator : public MathNode
{
public:
    //! Operator implementation, this will be evaluated during `Evaluate` calls
    std::function<double(double, double)> op;
    //! Operator priority, used to create AST
    OperatorPriority priority;
    /**
     * default constructor
     * @param name
     * @param fun
     * @param operatorPriority
     */
    Operator(const std::string& name, std::function<double(double, double)> fun, OperatorPriority operatorPriority)
        : MathNode(name)
        , op(std::move(fun))
        , priority(operatorPriority) {
        type = MathNodeType::NodeType_Operator;
    }

    /**
     * copy constructor
     * @param other
     */
    Operator(const Operator& other)
        : MathNode(other) {
        op       = other.op;
        priority = other.priority;
    }

    /**
     * evaluates op(left, right)
     * @return
     */
    [[nodiscard]] double Evaluate() const override {
        assert(left != nullptr && right != nullptr);
        return op(left->Evaluate(), right->Evaluate());
    }
};

/**
 * Operator factory, returns shared_ptr to operator with given type
 * @param type
 * @return
 */
std::shared_ptr<Operator> GenerateOperator(OperatorType type);

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

[[nodiscard]] bool isConstant(const std::string& in) {
    std::unordered_set<std::string> funList;
    for(const auto& elem : DefaultSymbols) { funList.insert(elem.first); }
    return funList.find(in) != funList.end();
}

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

/**
 * Function representation as node.
 *
 * Current implementation represents single operand functions, like
 * \f[
 *  \sqrt{x} \quad \text{ or } \quad \log{x}
 * \f]
 */
class Function : public Operator
{
public:
    /**
     * Single operand default constructor
     * @param val string representation
     * @param fun single value evaluation function
     */
    Function(const std::string& val, const std::function<double(double)>& fun)
        : Operator(
        val, [fun](double a, [[maybe_unused]] double b) { return fun(a); }, OperatorPriority::OPClassFunction) {
        connectionType = NodeConnectionType::ConnectionType_Left;
    }

    /**
     * Two operand default constructor
     * @param val string representation
     * @param fun two value evaluation function
     *
     * TODO: enable parsing for this kind of function!
     */
    Function(const std::string& val, const std::function<double(double, double)>& fun)
        : Operator(val, fun, OperatorPriority::OPClassFunction) {
        connectionType = NodeConnectionType::ConnectionType_Dual;
        std::cerr << "This can't be parsed yet!!!!!!" << std::endl;
    }

    [[nodiscard]] double Evaluate() const override {
        if(connectionType == NodeConnectionType::ConnectionType_Left) return op(left->Evaluate(), 0.0);
        else if(connectionType == NodeConnectionType::ConnectionType_Dual)
            return op(left->Evaluate(), right->Evaluate());
        return NAN;
    }

    // TODO: add beautified string representation
};
/**
 * Default functions for parser
 */
const std::map<std::string, std::shared_ptr<Function>> DefaultFunctions = {
    { "sqrt", std::make_shared<Function>("sqrt", [](double a) { return sqrt(a); }) },
    { "log", std::make_shared<Function>("log", [](double a) { return log(a); }) },
    { "sin", std::make_shared<Function>("sin", [](double a) { return sin(a); }) },
    { "cos", std::make_shared<Function>("cos", [](double a) { return cos(a); }) },
};
/**
 * Helper function to test if string is function representation
 * @param in
 * @return
 */
[[nodiscard]] bool isFunction(const std::string& in) {
    std::unordered_set<std::string> funList;
    for(const auto& elem : DefaultFunctions) { funList.insert(elem.first); }
    return funList.find(in) != funList.end();
}

/**
 * Representation of mathematical statement
 */
class Equation
{
public:
    //! storage for containing symbols
    std::vector<std::shared_ptr<Symbolic>> symbols;

    //! Holds the base node of the abstract syntax tree
    std::shared_ptr<MathNode> baseNode = nullptr;

    /**
     * empty default constructor
     */
    Equation() { }

    /**
     * default constructor for char* representation
     * @param val
     */
    explicit Equation(const char* val) {
        std::string eq(val);
        baseNode = createAST(eq);
    }
    /**
     * default constructor for string representation
     * @param val
     */
    explicit Equation(const std::string& val) { baseNode = createAST(val); }

    /**
     * Prints equation into std::cout
     */
    void Print() {
        std::cout << baseNode;
        std::cout << std::endl;
    }

    /**
     * var arg method to evaluate equation using variable amount of symbols,
     * yet if called with parameters, all parameters need to be present.
     *
     * **Needs to be called with number symbols as number parameters**
     *
     * @tparam VArgs variable amount of arguments
     * @param args arguments passed for symbols, with index representing index of symbol in symbols
     * @return Evaluation of equation
     */
    template<typename... VArgs>
    double operator()(VArgs... args) {
        assert(sizeof...(args) == symbols.size());
        SetSymbols(0, args...);
        return baseNode->Evaluate();
    }
    /**
     * Evaluate equation with preset values
     * @return
     */
    double operator()() { return baseNode->Evaluate(); }

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
     * Helper shortcut to check for existence of symbolic within equation
     * setter for evaluation values for symbolic nodes using a vector of values
     *
     * @param values vector containing values for symbols
     */
    void SetSymbols(const std::vector<double>& values) {
        assert(values.size() == symbols.size());

        for(size_t i = 0; i < symbols.size(); ++i) { symbols[i]->evaluationValue = values[i]; }
    }

    /**
     * Helper shortcut to check for existence of symbolic within equation
     * @param sym symbol to search for
     * @return sym already registered?
     */
    [[nodiscard]] bool HasSymbol(const std::shared_ptr<Symbolic>& sym) const { return HasSymbol(symbols, sym); }

    /**
     * Static helper to check for existence of symbolic within a container
     * @param container
     * @param sym
     * @return
     */
    [[nodiscard]] bool
    HasSymbol(const std::vector<std::shared_ptr<Symbolic>>& container, const std::shared_ptr<Symbolic>& sym) const {
        for(const auto& elem : container) {
            if(bool(strcmp(elem->value, sym->value) == 0) && elem->isNegative == sym->isNegative) return true;
        }
        return false;
    }

    /**
     * Helper to recreate string representation based on tree
     * @return
     */
    [[nodiscard]] std::string GetString() const { return baseNode->GetString(); }

    /**
     * Helper method to display ASTree.
     */
    void PrintTree() const {
        size_t row   = 0;
        size_t elems = GetDepth(baseNode, row)+1;
        std::vector<std::vector<std::string>> levels(elems, std::vector<std::string>(elems));
        row       = 0;
        size_t col       = 0;
        PrintNode(baseNode, levels, row, col);
        std::vector<std::vector<std::string>> realLevels(elems, std::vector<std::string>(elems*2));
        /**
         * Add padding to levels, count empty elements on right side, then append them on the left side
         */
        bool eol = false;
        std::vector<size_t> padding(elems);
        size_t index = 0;
        for(const auto& line : levels){
            for(const auto& elem : line) {
                if(elem == "  " || elem.empty()){
                    if(eol){
                        padding[index]++;
                    }
                    eol = true;
                } else {
                    padding[index] = 0;
                }
            }
            index++;
        }

        for(size_t paddIndex = 0; paddIndex < elems; ++paddIndex){
            for(index = 0; index < elems + padding[paddIndex]; ++index) {
                if(index < padding[paddIndex]){
                    std::cout << "  ";
                }
                else {
                    std::cout << levels[paddIndex][index - padding[paddIndex]];
                }
            }
            std::cout << std::endl;
        }
    }

    /**
     * Generates left aligned string representation of node in tree leave form according to their connection types
     * @param node
     */
    void PrintNode(const std::shared_ptr<MathNode> &node, std::vector<std::vector<std::string>>& levels, const size_t& row, const size_t& column) const {
        switch(node->connectionType) {
            case NodeConnectionType::ConnectionType_Dual:
                {
                    /*      op
                     *    /    \
                     *  left   right
                     */
                    levels[row][column] += "  ";
                    levels[row][column+1] += node->value;
                    levels[row][column+2] += "  ";
                    levels[row +1][column] += "  /";
                    levels[row +1][column+2] += "  \\";
                    auto childIndex = row + 2;
                    levels[childIndex][column+1] += "  ";
                    PrintNode(node->left, levels, childIndex, column);
                    PrintNode(node->right,levels, childIndex, column+2);
                    levels[childIndex][column+2] += "  ";
                }
                break;
            case NodeConnectionType::ConnectionType_Left:
                {
                    /*      fun
                     *    /
                     *  left
                     */
                    levels[row][column] += "  ";
                    levels[row][column+1] += node->value;
                    levels[row][column+2] += "  ";
                    levels[row +1][column] += "  /";
                    levels[row +1][column+2] += "  ";
                    auto childIndex = row + 2;
                    levels[childIndex][column+1] += "  ";
                    levels[childIndex][column+2] += "  ";
                    PrintNode(node->left, levels, childIndex, column);
                }
                break;
            case NodeConnectionType::ConnectionType_Right:
            {
                /*      fun
                 *         \
                 *         right
                 */
                levels[row][column] += "  ";
                levels[row][column+1] += node->value;
                levels[row][column+2] += "  ";
                levels[row +1][column] += "  ";
                levels[row +1][column+2] += "  \\";
                auto childIndex = row + 2;
                levels[childIndex][column] += "  ";
                levels[childIndex][column+1] += "  ";
                PrintNode(node->right, levels, childIndex, column+2);
                levels[childIndex][column+2] += "  ";
            }
                break;
            case NodeConnectionType::ConnectionType_None:
                {
                    //value
                    levels[row][column] += std::string("  ") + node->value;
                }
                break;
            case NodeConnectionType::ConnectionType_Unknown:
            default:
                break;
        }
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

    /**
     * Connects left and right using op, this essentially
     * takes both sides baseNode and connects both trees, then
     * returns a new equation object.
     * Dude! It even generates a unique set of symbols!
     * @param left left equation
     * @param right right equation
     * @param op operator to connect with
     * @return chained equation object
     */
    static Equation Chain(const Equation& left, const Equation& right, const std::shared_ptr<Operator>& op) {
        Equation out;
        op->left     = left.baseNode;
        op->right    = right.baseNode;
        out.baseNode = op;
        out.symbols  = buildSymbolSuperSet(left.symbols, right.symbols);
        return out;
    }

    /**
     * Simplifies an equation according to mathematical rules.
     *
     * @return simplified equation
     */
    [[nodiscard]] Equation Simplify() const {
        Equation out = *this;

        auto newBase = baseNode;
        // Early exit for constants
        switch(newBase->type) {
            case NodeType_Symbolic:
            case NodeType_Numeric:
            case NodeType_DefaultSymbol:
            case NodeType_Any:
                break;
            case NodeType_Operator:
            case NodeType_Parentheses:
            case NodeType_Functional:
                out.baseNode = SimplifyTree(newBase);
        }

        return out;
    }

private:
    /**
     *
     * @param node
     * @return
     */
    [[nodiscard]] std::shared_ptr<MathNode> SimplifyTree(const std::shared_ptr<MathNode> &node) const {
        auto out = SimplifyLevels(node);
        out = ResolveLevels(out);

        return out;
    }
    /**
     *
     * @param node
     * @return
     */
    [[nodiscard]] std::shared_ptr<MathNode> ResolveLevels(const std::shared_ptr<MathNode> &node) const {
        if(node->type == NodeType_Numeric || node->type == NodeType_Symbolic)
            return node;

        auto out = node;
        if(out->type == NodeType_Operator){
            auto op = GetOperator(out->value);
            // resolve line operators +/-
            if(op->priority == OPClassLine){
                // left value is numeric. Search for left side of right operand
                if(out->left->type == NodeType_Numeric){
                    auto rightNode = out->right;
                    while(rightNode != nullptr){
                        if(rightNode->type == NodeType_Numeric){
                            out = Apply(rightNode, op, out->left->Evaluate(), true);
                            break;
                        }
                        rightNode = rightNode->left;
                    }
                }
                else if(out->right->type == NodeType_Numeric){
                    auto leftNode = out->left;
                    while(leftNode != nullptr){
                        if(leftNode->type == NodeType_Numeric){
                            out = Apply(leftNode, op, out->right->Evaluate(), false);
                            break;
                        }
                        leftNode = leftNode->right;
                    }
                }
            }
        }
        return out;
    }
    /**
     * Simplifies Equation Scopes.
     *
     * Within a scope the following rules are applied:
     *
     *   - Functions:
     *      Functions with numeric values as left hand nodes can be simplified to numeric nodes
     *
     *   - Line operators:
     *      Within same tree it is allowed to combine nodes.
     *      Same symbols can be simplified as Number*Symbol leaf.
     *
     *   - Dot operators:
     *      Numeric values can be combined.
     *
     * @param node tree to simplify
     * @return simplified tree
     */
    [[nodiscard]] std::shared_ptr<MathNode> SimplifyLevels(const std::shared_ptr<MathNode> &node) const {
        auto newBase = node;

        switch(newBase->type) {
            case NodeType_Any:
            case NodeType_Parentheses:
                break;
            case NodeType_Symbolic:
            case NodeType_Numeric:
            case NodeType_DefaultSymbol:
            case NodeType_Operator:
                if(newBase->left->type == NodeType_Operator
                   || newBase->left->type == NodeType_Functional){
                    newBase->left = SimplifyTree(newBase->left);
                }
                if(newBase->right->type == NodeType_Operator
                   || newBase->right->type == NodeType_Functional){
                    newBase->right = SimplifyTree(newBase->right);
                }
                if(newBase->left->type == NodeType_Numeric
                   && newBase->right->type == NodeType_Numeric){
                    return std::make_shared<Number>(std::to_string(newBase->Evaluate()));
                }
                break;
            case NodeType_Functional:
                if(newBase->left->type == NodeType_Operator
                   || newBase->left->type == NodeType_Functional){
                    newBase->left = SimplifyTree(newBase->left);
                }
                if(newBase->left->type == NodeType_Numeric){
                    return std::make_shared<Number>(std::to_string(newBase->Evaluate()));
                }
                break;
        }
        return newBase;
    }

    [[nodiscard]] std::shared_ptr<MathNode> Apply(const std::shared_ptr<MathNode>& node, const std::shared_ptr<Operator>& op, const double& val, bool isLeft) const {
        auto out = node;
        switch(out->connectionType) {
            case ConnectionType_Dual:
                if(isLeft)out->left = Apply(out->left, op, val, true);
                else out->right = Apply(out->right, op, val, false);
                break;
            case ConnectionType_Left:
                if(isLeft)out->left = Apply(out->left, op, val, true);
                break;
            case ConnectionType_Right:
                if(!isLeft) out->right = Apply(out->right, op, val, false);
                break;
            case ConnectionType_None:
            {
                if(out->type == NodeType_Numeric){
                    if(isLeft) return std::make_shared<Number>(std::to_string(op->op(out->Evaluate(), val)));
                    else return std::make_shared<Number>(std::to_string(op->op(val, out->Evaluate())));
                }
            }
                break;
            case ConnectionType_Unknown: break;
        }
        return out;
    }

    /**
     * helper method to generate superset of given vectors
     * @param a set of symbols
     * @param b other set of symbols
     * @return merged unique set of symbols
     */
    static std::vector<std::shared_ptr<Symbolic>> buildSymbolSuperSet(
    const std::vector<std::shared_ptr<Symbolic>>& a, const std::vector<std::shared_ptr<Symbolic>>& b) {
        std::vector<std::shared_ptr<Symbolic>> out = a;
        auto testIsInContainer                     = [&out](const std::shared_ptr<Symbolic>& sym) {
            for(const auto& elem : out) {
                if(bool(strcmp(elem->value, sym->value) == 0) && elem->isNegative == sym->isNegative) return true;
            }
            return false;
        };


        size_t numElements = out.size();

        for(const auto& i : b) {
            if(!testIsInContainer(i)) {
                out.push_back(i);
                numElements++;
            }
        }

        return out;
    }
    /**
     * Lexer function to determine first connecting operator
     * @param valString
     * @return
     */
    static std::shared_ptr<Operator> GetOperator(const std::string& valString) {
        if(valString.empty()) { return {}; }
        if(valString.size() > 1) { return GetFunction(valString); }
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
     * Lexer function to detect a Function node.
     * @param valString element of DefaultFunctions vector
     * @return
     */
    static std::shared_ptr<Function> GetFunction(const std::string& valString) {
        auto elem = DefaultFunctions.find(valString);
        return (elem != DefaultFunctions.end()) ? elem->second : nullptr;
    }

    /**
     * Recursively split of input string into function/symbol/operator/numerical substrings.
     * @param processString
     * @return
     */
    std::vector<std::string> splitFunctionsOrElementwise(const std::string& processString) {
        std::vector<std::string> out;
        auto eq = processString;

        std::vector<std::string> functionNames;
        functionNames.reserve(DefaultFunctions.size());
        for(const auto& elem : DefaultFunctions) { functionNames.push_back(elem.first); }
        auto res = extractObjects(eq, functionNames);
        out.insert(out.end(), res.begin(), res.end());

        std::vector<std::string> constantNames;
        constantNames.reserve(DefaultSymbols.size());
        for(const auto& elem : DefaultSymbols) { constantNames.push_back(elem.first); }
        res = extractObjects(eq, constantNames);
        out.insert(out.end(), res.begin(), res.end());

        if(!eq.empty()) {
            auto remainingPart = split(eq);
            out.insert(out.end(), remainingPart.begin(), remainingPart.end());
        }
        return out;
    }

    /**
     * extracts given objects from given equation string
     * @param eq equation string
     * @param container container containing objects to search for
     * @return extracted objects
     */
    std::vector<std::string> extractObjects(std::string& eq, const std::vector<std::string> &container){
        std::vector<std::string> out;
        bool keepRunning = true;
        while(keepRunning) {
            for(const auto& funStr : container) {
                auto start  = eq.find(funStr);
                keepRunning = false;
                if(start != std::string::npos) {
                    if(start > 0) {
                        auto ls = splitFunctionsOrElementwise(eq.substr(0, start));
                        out.insert(out.end(), ls.begin(), ls.end());
                    }
                    out.push_back(funStr);
                    auto endPos = start + funStr.size();
                    if(endPos < eq.size()) {
                        // remaining right side, continue
                        eq          = eq.substr(endPos, eq.size() - endPos);
                        keepRunning = true;
                    } else {
                        // processing done, return
                        keepRunning = false;
                    }
                }
            }
        }
        return out;
    }

    /**
     * Splits an equation string into a vector of strings
     *
     * @param processString equation string to split
     * @return vector with equation elements
     */
    std::vector<std::string> splitEquation(const std::string& processString) {
        std::vector<std::string> out;
        std::cmatch m;

        auto numberRegex        = GetRegex(MathNodeType::NodeType_Numeric);
        auto symbolRegex        = GetRegex(MathNodeType::NodeType_Symbolic);
        auto operatorRegex      = GetRegex(MathNodeType::NodeType_Operator);

        auto splitEq = split(processString, ' ');

        // bigger as -x?
        if(splitEq.size() == 1) {
            out = splitFunctionsOrElementwise(splitEq[0]);
        } else {
            for(const auto& elem : splitEq) {
                // only allow full matches
                if((elem.size() == 1)
                   || (
                        (std::regex_match(elem.c_str(), m, symbolRegex) && m.str().size() == elem.size())
                        || (std::regex_match(elem.c_str(), m, numberRegex) && m.str().size() == elem.size())
                        || (std::regex_match(elem.c_str(), m, operatorRegex) && m.str().size() == elem.size())
                   )){
                    out.push_back(elem);
                } else {
                    auto withFun = splitFunctionsOrElementwise(elem);
                    out.insert(out.end(), withFun.begin(), withFun.end());
                }
            }
        }

        return out;
    }

private:
    /**
     * tests if value is number
     * @param in
     * @return
     */
    static bool isNumber(const std::string& in) {
        static auto numberRegex    = GetRegex(MathNodeType::NodeType_Numeric);
        std::cmatch m;
        return std::regex_match(in.c_str(), m, numberRegex);
    };
    /**
     * tests if value is symbol
     * @param in
     * @return
     */
    static bool isSymbol(const std::string& in) {
        static auto symbolRegex    = GetRegex(MathNodeType::NodeType_Symbolic);
      std::cmatch m;
      return std::regex_match(in.c_str(), m, symbolRegex);
    };
    /**
     * test if value is operator
     * @param in
     * @return
     */
    static bool isOperator(const std::string& in) {
        static auto operatorRegex =  GetRegex(MathNodeType::NodeType_Operator);
      std::cmatch m;
      return std::regex_match(in.c_str(), m, operatorRegex);
    };
    /**
     * test if value is anything processable
     * @param in
     * @return
     */
    static bool isAny (const std::string& in) {
        static auto anyRegex = GetRegex(MathNodeType::NodeType_Any);
        std::cmatch m;
        return std::regex_match(in.c_str(), m, anyRegex);
    };
    /**
     * test if value is parentheses open
     * @param in
     * @return
     */
    static bool isParenthesesOpen(const std::string& in) { return in.find('(') != std::string::npos; };
    /**
     * test if value is parentheses close
     * @param in
     * @return
     */
    static bool isParenthesesClose(const std::string& in) { return in.find(')') != std::string::npos; };

public:
    /**
     * Creates A(bstract)S(yntax)T(ree) using shunting-yard algorithm, thanks Mr. Dijkstra :)
     * https://en.wikipedia.org/wiki/Shunting-yard_algorithm
     *
     * @param processString
     * @return
     */
    std::shared_ptr<MathNode> createAST(const std::string& processString) {
        auto regex_end = std::sregex_iterator();
        std::vector<std::string> operatorStack;
        std::vector<std::shared_ptr<MathNode>> operandStack;

        bool prevWasOperator = true;
        bool nextIsNegative  = false;
        auto eqParts         = splitEquation(processString);
        for(const auto& c : eqParts) {
            if(isNumber(c)) {
                auto sym        = std::make_shared<Number>(c);
                sym->isNegative = nextIsNegative;
                nextIsNegative  = false;
                operandStack.push_back(sym);
            } else if(isConstant(c)) {
                auto sym        = DefaultSymbols[c];
                sym->isNegative = nextIsNegative;
                nextIsNegative  = false;
                operandStack.push_back(sym);
            } else if(isSymbol(c)) {
                //TODO: determine if symbol is default symbol like pi or e
                //	then reuse existing
                auto symbol        = std::make_shared<Symbolic>(c);
                symbol->isNegative = nextIsNegative;
                nextIsNegative     = false;
                operandStack.push_back(symbol);
                if(!HasSymbol(symbol)) symbols.push_back(symbol);
            } else if(isParenthesesOpen(c) || isFunction(c)) {
                operatorStack.push_back(c);
            } else if(isOperator(c)) {
                auto currentOp = GetOperator(c);
                if(prevWasOperator && currentOp->value[0] == '-') {
                    nextIsNegative = true;
                } else {
                    processCurrentOP(currentOp, operatorStack, operandStack);
                    operatorStack.push_back(c);
                }
            } else if(isParenthesesClose(c)) {
                rearrangeStack(operatorStack, operandStack);
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

            auto currentOp = GetOperator(stackTop);
            if(currentOp != nullptr) {
                if(currentOp->connectionType == NodeConnectionType::ConnectionType_Dual) {
                    currentOp->right = operandStack[operandStack.size() - 1];
                    operandStack.pop_back();
                }

                currentOp->left = operandStack[operandStack.size() - 1];
                operandStack.pop_back();

                operandStack.push_back(currentOp);
            }
        }

        return !operandStack.empty() ? operandStack.front() : nullptr;
    }

    /**
     * process current operator until end of equation, next operator with higher priority or closing parentheses
     * @param operatorStack current operator stack object
     * @param operandStack current operand stack
     * @param currentOp the operator to process
     */
    void processCurrentOP(const std::shared_ptr<Operator>& currentOp, std::vector<std::string>& operatorStack, std::vector<std::shared_ptr<MathNode>>& operandStack){
        if(!operatorStack.empty()) {
            auto stackTop = GetOperator(operatorStack[operatorStack.size() - 1]);
            while(stackTop != nullptr && stackTop->priority >= currentOp->priority
                  && stackTop->value[0] != '(') {
                operatorStack.pop_back();
                if(stackTop->connectionType == NodeConnectionType::ConnectionType_None) {
                    if(!operatorStack.empty())
                        stackTop = GetOperator(operatorStack[operatorStack.size() - 1]);
                    continue;
                }

                if(stackTop->connectionType == NodeConnectionType::ConnectionType_Dual) {
                    // inverse order, first right then left
                    stackTop->right = operandStack[operandStack.size() - 1];

                    operandStack.pop_back();
                }
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
    }

    /**
     * rearranges stack until end of equation of opening parentheses
     * @param operatorStack operator stack object
     * @param operandStack operand stack object
     */
    void rearrangeStack(std::vector<std::string>& operatorStack, std::vector<std::shared_ptr<MathNode>>& operandStack){
        auto stackTop = operatorStack[operatorStack.size() - 1];
        while(!isParenthesesOpen(stackTop)) {
            // handle operator
            auto currentOp = GetOperator(stackTop);
            if(currentOp != nullptr) {
                if(currentOp->connectionType == NodeConnectionType::ConnectionType_Dual) {
                    currentOp->right = operandStack[operandStack.size() - 1];
                    operandStack.pop_back();
                }
                currentOp->left = operandStack[operandStack.size() - 1];
                operandStack.pop_back();

                operandStack.push_back(currentOp);
            }
            if(operatorStack.size() > 1) {
                operatorStack.pop_back();

                stackTop = operatorStack[operatorStack.size() - 1];
            } else
                break;
        }
        operatorStack.pop_back();
    }
};
