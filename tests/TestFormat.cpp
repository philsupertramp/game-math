#include "Test.h"
#include <math/format.h>

class FormatTestCase : public Test
{
    bool TestFormat() {
        auto out = format("%d, 2, 3", 1);

        AssertEqual(out, "1, 2, 3");
        return true;
    }

    bool TestFormatOverflow() {
        char foo[516]           = "%d.";
        std::string fooExpected = "1.";
        for(size_t i = 0; i < 512; ++i) {
            foo[i + 3] = '.';
            fooExpected += ".";
        }
        auto out = format(foo, 1);

        AssertEqual(out, fooExpected);

        return true;
    }

    bool TestStrip() {
        std::string in = "1 + 2 + 3 - 5(x*2 + 3)";
        AssertEqual(strip(in), "1+2+3-5(x*2+3)");

        return true;
    }

    bool TestSplit() {
        std::string in = "Hey there, my friend.";
        auto out       = split(in, ' ');

        std::vector<std::string> ref = { "Hey", "there,", "my", "friend." };

        for(size_t i = 0; i < ref.size(); ++i) { AssertEqual(ref[i], out[i]); }

        std::string in2               = "Hey";
        auto out2                     = split(in2);
        std::vector<std::string> ref2 = { "H", "e", "y" };
        for(size_t i = 0; i < ref2.size(); ++i) { AssertEqual(ref2[i], out2[i]); }
        return true;
    }

public:
    void run() override {
        TestFormat();
        TestFormatOverflow();
        TestStrip();
        TestSplit();
    }
};

int main() {
    FormatTestCase().run();
    return 0;
}
/**
 * \example TestFormat.cpp
 * This is an example on how to use the format.h class.
 */