/**
 * TODO:
 * - split into sub-files
 * - create header/source files
 * - reduce complexity
 * - finalize simplification
 */

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
#include "EquationParser.h"
#include "MathNode.h"
#include "Operand.h"
#include "Operator.h"


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
        EquationParser parser(eq);
        baseNode = parser.createAST();
        symbols = parser.symbols;
    }
    /**
     * default constructor for string representation
     * @param val
     */
    explicit Equation(const std::string& val) {
        EquationParser parser(val);
        baseNode = parser.createAST();
        symbols = parser.symbols;
    }

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
        out.symbols  = EquationParser::buildSymbolSuperSet(left.symbols, right.symbols);
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
     * TODO
     * @param node
     * @return
     */
    [[nodiscard]] std::shared_ptr<MathNode> SimplifyTree(const std::shared_ptr<MathNode> &node) const {
        auto out = SimplifyLevels(node);
        out = ResolveLevels(out);

        return out;
    }

    /**
     *TODO
     * @param node
     * @return
     */
    [[nodiscard]] std::shared_ptr<MathNode> ResolveLevels(const std::shared_ptr<MathNode> &node) const {
        if(node->type == NodeType_Numeric || node->type == NodeType_Symbolic)
            return node;

        auto out = node;
        if(out->type == NodeType_Operator){
            auto op = EquationParser::GetOperator(out->value);
            // resolve line operators +/-
            switch(op->priority){
                case OPClassLine:
                case OPClassDot:
                    {
                        resolveOP(out, op);
                        break;
                    }
                case OPClassUnknown:
                case OPClassParentheses:
                case OPClassFunction: break;
            }
        }
        return out;
    }

    /**
     * TODO
     * @param out
     * @param op
     */
    void resolveOP(std::shared_ptr<MathNode>& out, const std::shared_ptr<Operator>& op) const {
        // left value is numeric. Search for left side of right operand
        if(out->left->type == NodeType_Numeric){
            auto rightNode = out->right;
            while(rightNode != nullptr){
                if(rightNode->type == NodeType_Numeric){
                    out = ApplyOperator(rightNode, op, out->left->Evaluate(), true);
                    break;
                }
                rightNode = rightNode->left;
            }
        }
        else if(out->right->type == NodeType_Numeric){
            auto leftNode = out->left;
            while(leftNode != nullptr){
                if(leftNode->type == NodeType_Numeric){
                    out = ApplyOperator(leftNode, op, out->right->Evaluate(), false);
                    break;
                }
                leftNode = leftNode->right;
            }
        }
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
        switch(node->type) {
            case NodeType_Any:
            case NodeType_Parentheses:
                break;
            case NodeType_Symbolic:
            case NodeType_Numeric:
            case NodeType_DefaultSymbol:
            case NodeType_Operator:
            case NodeType_Functional:
                return simplifyOP(node);
        }
        return node;
    }

    /**
     * Simplifies operation tree
     * @param node operator node (-tree) to simplify
     * @return simplified tree with [num_nodes_in >= num_nodes_out]
     */
    [[nodiscard]] std::shared_ptr<MathNode> simplifyOP(const std::shared_ptr<MathNode> &node) const {
        if(node->left->type == NodeType_Operator
           || node->left->type == NodeType_Functional){
            node->left = SimplifyTree(node->left);
        }
        if(node->right != nullptr) {
            if(node->right->type == NodeType_Operator || node->right->type == NodeType_Functional) {
                node->right = SimplifyTree(node->right);
            }
            if(node->left->type == NodeType_Numeric && node->right->type == NodeType_Numeric) {
                return std::make_shared<Number>(std::to_string(node->Evaluate()));
            }
        }
        return node;
    }

    /**
     * Applies numeric operation op like
     * if isLeft:
     *  node (op) val
     * else:
     *  val (op) node
     *
     * @param node target to apply op on
     * @param op operator to apply
     * @param val numerical value to apply
     * @param isLeft signalize node is left side of OP
     * @return evaluated operator node
     */
    [[nodiscard]] std::shared_ptr<MathNode> ApplyOperator(const std::shared_ptr<MathNode>& node, const std::shared_ptr<Operator>& op, const double& val, bool isLeft) const {
        auto out = node;
        switch(out->connectionType) {
            case ConnectionType_Dual:
                if(isLeft)out->left = ApplyOperator(out->left, op, val, true);
                else out->right = ApplyOperator(out->right, op, val, false);
                break;
            case ConnectionType_Left:
                if(isLeft)out->left = ApplyOperator(out->left, op, val, true);
                break;
            case ConnectionType_Right:
                if(!isLeft) out->right = ApplyOperator(out->right, op, val, false);
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
};
