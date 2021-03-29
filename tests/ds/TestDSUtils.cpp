#include "../Test.h"
#include <cassert>


class DSUtilsTestCase : public Test
{
    bool TestUtils() {
        // TODO
        return true;
    }

public:
    void run(bool _extended) override { TestUtils(); }
};

int main(int nargs, char* argv[]) { DSUtilsTestCase().run(false); }