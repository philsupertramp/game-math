#include "../../Test.h"
#include <math/numerics/lin_alg/backwardSub.h>
#include <vector>


class BackwardSubTestCase : public Test
{
    bool TestBackwardSub() {
        std::vector<std::vector<float>> R = { { 15.0f, 2.0f }, { 16.0f, 46.0f } };
        std::vector<float> b              = { 2.0f, 3.0f };
        auto x                            = backwardSub(R, b);

        std::vector<float> LsgX = { 0.124637686f, 0.0652173907f };

        for(size_t i = 0; i < 2; i++) { assert(x[i] == LsgX[i]); }


        R = { { 4, 2, 3 }, { 0, 1, -0.5 }, { 0, 0, 1 } };
        b = { 2, 0, 1 };
        x = backwardSub(R, b);

        LsgX = { -0.5f, 0.5f, 1.0f };

        for(size_t i = 0; i < 3; i++) { assert(x[i] == LsgX[i]); }

        return true;
    }

    bool TestErrors() {
        std::vector<std::vector<float>> R = { { 15.0f }, { 16.0f } };
        std::vector<float> b              = { 2.0f, 3.0f };

        auto x = backwardSub(R, b);
        assert(x.size() == 0);

        std::vector<std::vector<float>> R2 = { { 15.0f, 1.0f } };
        std::vector<float> b2              = { 2.0f, 3.0f };

        auto x2 = backwardSub(R2, b2);
        assert(x2.size() == 0);
        return true;
    }

public:
    void run(bool _extended) override {
        TestBackwardSub();
        TestErrors();
    }
};

int main(int nargs, char* argv[]) { BackwardSubTestCase().run(false); }