#include "../Test.h"
#include <math/ds.h>


class DSUtilsTestCase : public Test
{
    bool TestUtils() {
        // TODO
        return true;
    }

public:
    void run() override { TestUtils(); }
};

int main() { DSUtilsTestCase().run(); }