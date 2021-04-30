#pragma once


#include <sstream>
#include <regex>


class MathNode
{
    size_t valSize = 0;
public:

    MathNode* left = nullptr;
    MathNode* right = nullptr;
    char* value{};

    explicit MathNode(const std::string& val)
    {
        value = new char[val.size() + 1];
        std::copy(val.begin(), val.end(), value);
        value[val.size()] = '\0'; // don't forget the terminating 0
        valSize = val.size() + 1;
    }
    MathNode(const MathNode& other)
    {
        std::memcpy(this->value, other.value, other.valSize * sizeof(char));
    }
};
class Symbolic
: public MathNode
{

};

class Operator
: public MathNode
{
public:
    Operator(const std::string& name)
    : MathNode(name)
    {}
};

namespace Operators {
    Operator* ADDITION       = new Operator("+");
    Operator* SUBTRACTION    = new Operator("-");
    Operator* MULTIPLICATION = new Operator("*");
    Operator* DIVISION       = new Operator("/");
    Operator* POWER          = new Operator("^");
    Operator* ROOT           = new Operator("sqrt");
}

class Number
: public MathNode
{
public:
    Number(const std::string& val)
    : MathNode(val)
    {}
};

class Equation
{
    std::vector<MathNode*> nodes;
public:
    explicit Equation(const char* val){
        std::string eq(val);

        Generate(eq);
    }
    void Generate(const std::string& eq){
        auto openParentheses = eq.find('(');
        bool hasParentheses = openParentheses != eq.npos;
        MathNode* leftSide;
        MathNode* rightSide;
        std::regex symbol_regex("([\\w0-9]+)");
        std::regex number_regex("([0-9]+)");

        if(hasParentheses){
            // build parentheses-stack for left side
        }
        else {
            // build left side from beginning until first operator
            // allow early exit
            while(1) {
                auto addStart = eq.find(Operators::ADDITION->value);
                if(addStart != eq.npos) {
                    leftSide = Operators::ADDITION;
                    auto leftLeft = eq.substr(0, addStart-1);
                    {
                        auto num_begin = std::sregex_iterator(leftLeft.begin(), leftLeft.end(), number_regex);
                        auto num_end   = std::sregex_iterator();
                        if(std::distance(num_begin, num_end) > 1){
                            //error
                            std::cerr << "ERROR!!!!" << std::endl;
                        }
                        for(std::sregex_iterator i = num_begin; i != num_end; ++i) {
                            std::smatch match     = *i;
                            leftSide->left = new Number(match.str());
                        }
                    }
                    break;
                }
                eq.find(Operators::SUBTRACTION->value);
            }
        }
    }

    void Print(){
        for(auto node: nodes){
            std::cout << node->value;
        }
        std::cout << std::endl;
    }

};