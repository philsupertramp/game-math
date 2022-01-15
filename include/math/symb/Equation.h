/**
 * @file Equation.h
 *
 *
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

    //! degree of equation. Linear equation = 1, quadratic = 2, ...
    int degree = 1;

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
        symbols  = parser.symbols;
        degree   = GetDegree(baseNode);
    }
    /**
     * default constructor for string representation
     * @param val
     */
    explicit Equation(const std::string& val) {
        EquationParser parser(val);
        baseNode = parser.createAST();
        symbols  = parser.symbols;
        degree   = GetDegree(baseNode);
    }

    int GetDegree(const std::shared_ptr<MathNode>& node) {
        if(node == nullptr) return 0;

        if(node->type == NodeType_Operator) {
            if(node->value[0] == '^' && node->left->type == NodeType_Symbolic) { return (int)node->right->Evaluate(); }
            auto left  = GetDegree(node->left);
            auto right = GetDegree(node->right);
            return left > right ? left : right;
        }
        return 1;
    }

    /**
     * Prints equation into std::cout
     */
    void Print(std::ostream& ostream = std::cout) {
        ostream << baseNode;
        ostream << std::endl;
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
        // count depth
        size_t depth_counter = 0;
        size_t depth_value = GetDepth(baseNode, depth_counter);

        // calculate widths
        size_t right = 0;
        auto newBase = baseNode;
        while(newBase->right){
            right++;
            newBase = newBase->right;
        }
        newBase = baseNode;
        size_t left = 0;
        while(newBase->left){
            left++;
            newBase = newBase->left;
        }

        size_t width_value = (left + right + 1) * 2;
        //
        std::string out(depth_value * width_value, ' ');

        // fill the output string with empty values
        size_t row, col;
        for(row=0; row < depth_value; ++row){
            for(col=0; col < width_value; ++col){
            }
            out[row*width_value+col-1] = '\n';
        }
        auto center = (left + 1) * 2;
        out.replace(center, baseNode->valSize, baseNode->value);
        auto leftnode = baseNode->left;
        int current_row = 1;
        while(leftnode->left){
            auto pos = (current_row++) * width_value + (left--) * 2;
            out.replace(pos, leftnode->valSize, leftnode->value);
            leftnode = leftnode->left;
        }
        std::cout << "The tree:\n" << out << std::endl;
    }

    std::string PrintNode(const std::shared_ptr<MathNode>& node, size_t tabAnchor) const {
        size_t i;
        std::string out;
        for(i=0; i < tabAnchor; ++i){
            out += "\t";
        }
        out += node->value;

        return out;
    }

    /**
     * Generates left aligned string representation of node in tree leave form according to their connection types
     * @param node
     */
    void PrintNode(
    const std::shared_ptr<MathNode>& node,
    std::vector<std::vector<std::string>>& levels,
    const size_t& row,
    const size_t& column) const {
        switch(node->connectionType) {
            case NodeConnectionType::ConnectionType_Dual:
                {
                    /*      op
                     *    /    \
                     *  left   right
                     */
                    levels[row][column] += "  ";
                    levels[row][column + 1] += node->value;
                    levels[row][column + 2] += "  ";
                    levels[row + 1][column] += "  /";
                    levels[row + 1][column + 2] += "  \\";
                    auto childIndex = row + 2;
                    levels[childIndex][column + 1] += "  ";
                    PrintNode(node->left, levels, childIndex, column);
                    PrintNode(node->right, levels, childIndex, column + 2);
                    levels[childIndex][column + 2] += "  ";
                }
                break;
            case NodeConnectionType::ConnectionType_Left:
                {
                    /*      fun
                     *    /
                     *  left
                     */
                    levels[row][column] += "  ";
                    levels[row][column + 1] += node->value;
                    levels[row][column + 2] += "  ";
                    levels[row + 1][column] += "  /";
                    levels[row + 1][column + 2] += "  ";
                    auto childIndex = row + 2;
                    levels[childIndex][column + 1] += "  ";
                    levels[childIndex][column + 2] += "  ";
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
                    levels[row][column + 1] += node->value;
                    levels[row][column + 2] += "  ";
                    levels[row + 1][column] += "  ";
                    levels[row + 1][column + 2] += "  \\";
                    auto childIndex = row + 2;
                    levels[childIndex][column] += "  ";
                    levels[childIndex][column + 1] += "  ";
                    PrintNode(node->right, levels, childIndex, column + 2);
                    levels[childIndex][column + 2] += "  ";
                }
                break;
            case NodeConnectionType::ConnectionType_None:
                {
                    //value
                    levels[row][column] += std::string("  ") + node->value;
                }
                break;
            case NodeConnectionType::ConnectionType_Unknown:
            default: break;
        }
    }

    /**
     * Getter for max AST depth
     * @param node to check depth of
     * @param current_depth counter for current depth level
     * @return current depth if last node in branch, else max of left depth and right depth
     */
    size_t GetDepth(const std::shared_ptr<MathNode>& node, size_t current_depth) const {
        auto leftDep  = std::numeric_limits<size_t>::min();
        auto rightDep = std::numeric_limits<size_t>::min();
        if(node->left) { leftDep = GetDepth(node->left, current_depth+1); }
        if(node->right) { rightDep = GetDepth(node->right, current_depth+1); }

        return std::max(std::max(leftDep, current_depth), rightDep);
    }

    static size_t GetWidth(const std::shared_ptr<MathNode>& node) {
        auto newBase = node;
        size_t right = 0;
        while(newBase->right){
            right++;
            newBase = newBase->right;
        }
        newBase = node;
        size_t left = 0;
        while(newBase->left){
            left++;
            newBase = newBase->left;
        }
        // +1 for base node
        return right + left + 1;
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
     * Simplifies an equation in place according to mathematical rules.
     */
    void Simplify() {
        // Early exit for constants
        switch(baseNode->type) {
            case NodeType_Symbolic:
            case NodeType_Numeric:
            case NodeType_DefaultSymbol:
            case NodeType_Any: break;
            case NodeType_Operator:
            case NodeType_Parentheses:
            case NodeType_Functional: baseNode = SimplifyTree(baseNode);
            case NodeType_Operator_or_Parentheses: break;
        }
    }

private:
    /**
     * Simplify equation tree given by node
     * @param node tree to simplify
     * @return simplified tree
     */
    [[nodiscard]] std::shared_ptr<MathNode> SimplifyTree(const std::shared_ptr<MathNode>& node) const {
        auto out = node;
        if(
        out->type == NodeType_Numeric || out->type == NodeType_Operator || out->type == NodeType_DefaultSymbol
        || out->type == NodeType_Symbolic || out->type == NodeType_Functional) {
            out = simplifyOP(out);
        }

        // early exit
        if(out->type == NodeType_Numeric || out->type == NodeType_Symbolic) return out;

        if(out->type == NodeType_Operator) {
            auto op = EquationParser::GetOperator(out->value);
            // resolve line operators +/-
            if(op->priority == OPClassLine || op->priority == OPClassDot) { resolveOP(out, op); }
        }
        return out;
    }


    /**
     * resolves operator scope as far as possible
     * @param out output node
     * @param op operator scope to resolve
     */
    void resolveOP(std::shared_ptr<MathNode>& out, const std::shared_ptr<Operator>& op) const {
        // left value is numeric. Search for left side of right operand
        if(out->left->type == NodeType_Numeric) {
            auto rightNode = out->right;
            while(rightNode != nullptr) {
                if(rightNode->type == NodeType_Numeric) {
                    out = ApplyOperator(rightNode, op, out->left->Evaluate(), true);
                    break;
                }
                rightNode = rightNode->left;
            }
        } else if(out->right->type == NodeType_Numeric) {
            if(EquationParser::GetOperator(out->value)->priority != OperatorPriority::OPClassLine) { return; }
            auto leftNode = out->left;
            auto parent   = out;
            while(leftNode != nullptr) {
                if(leftNode->type == NodeType_Numeric) {
                    parent->right = ApplyOperator(leftNode, op, out->right->Evaluate(), false);
                    out           = out->left;
                    break;
                }
                parent   = leftNode;
                leftNode = leftNode->right;
            }
        }
    }

    /**
     * Simplifies operator scope
     *
     * Within a scope the following rules are applied:
     *
     *   - Functions:
     *      Functions with numeric values as left hand nodes can be simplified to numeric nodes
     *
     *   - Operators:
     *      Numeric values can be combined.
     *
     *
     * @param node operator node (-tree) to simplify
     * @return simplified tree with [num_nodes_in >= num_nodes_out]
     */
    [[nodiscard]] std::shared_ptr<MathNode> simplifyOP(const std::shared_ptr<MathNode>& node) const {
        std::shared_ptr<MathNode> nodeOut = node;
        // node is function, simplify f(x) -> y
        if(nodeOut->type == NodeType_Functional && nodeOut->left->type == NodeType_Numeric) {
            return std::make_shared<Number>(std::to_string(nodeOut->Evaluate()));
        }

        // left is Operator, simplify left := f(x) -> y
        if(nodeOut->left->type == NodeType_Operator || nodeOut->left->type == NodeType_Functional) {
            nodeOut->left = SimplifyTree(nodeOut->left);
        }

        // All done
        if(nodeOut->type != NodeType_Operator) return nodeOut;

        // from now on right is required
        assert(nodeOut->right != nullptr);

        // left and right are both Number, evaluate op
        if(nodeOut->left->type == NodeType_Numeric && nodeOut->right->type == NodeType_Numeric) {
            return std::make_shared<Number>(std::to_string(nodeOut->Evaluate()));
        }

        // right is Operator, simplify right := f(x) -> y
        if(nodeOut->right->type == NodeType_Operator || nodeOut->right->type == NodeType_Functional) {
            nodeOut->right = SimplifyTree(nodeOut->right);
        }

        return nodeOut;
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
    [[nodiscard]] std::shared_ptr<MathNode> ApplyOperator(
    const std::shared_ptr<MathNode>& node, const std::shared_ptr<Operator>& op, const double& val, bool isLeft) const {
        auto out = node;
        switch(out->connectionType) {
            case ConnectionType_Dual:
                if(isLeft) out->left = ApplyOperator(out->left, op, val, true);
                else
                    out->right = ApplyOperator(out->right, op, val, false);
                break;
            case ConnectionType_Left:
                if(isLeft) out->left = ApplyOperator(out->left, op, val, true);
                break;
            case ConnectionType_Right:
                if(!isLeft) out->right = ApplyOperator(out->right, op, val, false);
                break;
            case ConnectionType_None:
                {
                    if(out->type == NodeType_Numeric) {
                        if(isLeft) return std::make_shared<Number>(std::to_string(op->op(out->Evaluate(), val)));
                        else
                            return std::make_shared<Number>(std::to_string(op->op(val, out->Evaluate())));
                    }
                }
                break;
            case ConnectionType_Unknown: break;
        }
        return out;
    }
};

/**
 * \example symb/TestSymbolic.cpp
 * This is an example on how to use the symb module.
 */