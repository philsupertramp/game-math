#include "Test.h"
#include <math/format.h>

class FormatTestCase : public Test
{
    bool TestFormat() {
        auto out = format("%d, 2, 3", 1);

        AssertEqual(out, "1, 2, 3");
        return true;
    }

    bool TestOverflow() {
        char foo[516]           = "%d.";
        std::string fooExpected = "1.";
        for(size_t i = 0; i < 512; ++i) {
            foo[i + 3] = '.';
            fooExpected += ".";
        }
        auto out = format(foo, 1);

        std::cout << "out: " << out << std::endl << "exp: " << fooExpected << std::endl;
        AssertEqual(out, fooExpected);

        return true;
    }

public:
    void run() override {
        TestFormat();
        TestOverflow();
    }
};

int main() {
    FormatTestCase().run();
    return 0;
}