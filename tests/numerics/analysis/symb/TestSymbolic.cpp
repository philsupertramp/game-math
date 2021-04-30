#include "../../../Test.h"
#include <math/numerics/analysis/symb/Symbolic.h>


class SymbolicTestCase
: public Test
{
    bool TestSymbolic(){
        Equation symb("x + 1");
        symb.Print();
        return true;
    }
public:
    void run() override{
        TestSymbolic();
    }
};

int main(){
    SymbolicTestCase().run();
    return 0;
}