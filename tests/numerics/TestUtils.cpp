#include "../Test.h"
#include <math/numerics/utils.h>


class NumUtilsTestCase : public Test
{
    bool TestLinspace() {
        auto vec = linspace(0, 1, 3);
        assert(vec[0] == 0);
        assert(vec[1] == 0.5);
        assert(vec[2] == 1.0);
        return true;
    }

public:
    virtual void run() override { TestLinspace(); }
};

int main() {
    NumUtilsTestCase().run();
    return 0;
}
