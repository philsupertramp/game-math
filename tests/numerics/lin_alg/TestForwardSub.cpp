#include "../../Test.h"
#include <math/numerics/lin_alg/forwardSub.h>
#include <vector>


class ForwardSubTestCase : public Test
{
    bool TestForwardSub() {
        std::vector<std::vector<float>> L = { { 1.0f, 0.0f, 0.0f }, { 0.5f, 1.f, 0.f }, { 0.5f, 1.f, 1.f } };
        std::vector<float> b              = { 2.0f, 1.0f, 2.0f };
        auto x                            = forwardSub(L, b);

        std::vector<float> LsgX = { 2.0f, 0.0f, 1.0f };

        for(size_t i = 0; i < 3; i++) { assert(x[i] == LsgX[i]); }


        L = { { 1, 0, 0, 0 }, { 0.5, 1, 0, 0 }, { 0.5, 0.5, 1, 0 }, { 0.25, 0.5, 0.25, 1.0 } };
        b = { 2, 2, 2.5, -1.625 };
        x = forwardSub(L, b);

        LsgX = { 2.0f, 1.0f, 1.0f, -2.8750f };

        for(size_t i = 0; i < 3; i++) { assert(x[i] == LsgX[i]); }

        return true;
    }

    bool TestErrors() {
        std::vector<std::vector<float>> R = { { 15.0f }, { 16.0f } };
        std::vector<float> b              = { 2.0f, 3.0f };

        auto x = forwardSub(R, b);
        assert(x.size() == 0);

        std::vector<std::vector<float>> R2 = { { 15.0f, 1.0f } };
        std::vector<float> b2              = { 2.0f, 3.0f };

        auto x2 = forwardSub(R2, b2);
        assert(x2.size() == 0);
        return true;
    }

public:
    void run(bool _extended) override {
        TestForwardSub();
        TestErrors();
    }
};

int main() { ForwardSubTestCase().run(false); }