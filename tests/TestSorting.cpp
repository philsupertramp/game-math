#include "Test.h"
#include <math/sorting.h>


class SortTestCase : public Test
{
    bool TestSort() {
        std::vector<double> in       = { -1.0, 2, 5, 3, 9, 0, 4 };
        std::vector<double> expected = { -1, 0, 2, 3, 4, 5, 9 };

        AssertEqual(sort(in), expected);
        return true;
    }

public:
    virtual void run() { TestSort(); }
};


int main() {
    SortTestCase().run();
    return 0;
}
