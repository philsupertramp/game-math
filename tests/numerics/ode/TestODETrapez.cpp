#include "../../Test.h"
#include <math/numerics.h>

class ODETrapezTestCase : public Test
{
    bool TestOdeTrapez() {
        auto ode                     = []([[maybe_unused]] float t, std::vector<float> y) { return y; };
        std::vector<float> tInterval = { 0.0f, 2.0f };
        std::vector<float> y0        = { 1.0f };
        ODEOption options = {
            0.1f,
            1e-7,
            50,
            []([[maybe_unused]] float t, std::vector<float> y){ return std::vector<std::vector<float>>(y.size(), std::vector<float>(1)); }
        };

        auto foo = odeTrapez(ode, tInterval, y0, options);

        auto yResult = foo.first;
        auto tResult = foo.second;

        float tExpected[21] = {
            0.000000,
            0.100000,
            0.200000,
            0.300000,
            0.400000,
            0.500000,
            0.600000,
            0.700000,
            0.800000,
            0.900000,
            1.000000,
            1.100000,
            1.200000,
            1.300000,
            1.400000,
            1.500000,
            1.600000,
            1.700000,
            1.800000,
            1.900000,
            2.000000,
        };
        float yExpected[21][1] = {
            { 1.000000000000000 },
            { 1.105263157894737 },
            { 1.221606648199446 },
            { 1.350196821694125 },
            { 1.492322802925085 },
            { 1.649409413759305 },
            { 1.823031457312916 },
            { 2.014929505451117 },
            { 2.227027348130183 },
            { 2.461451279512307 },
            { 2.720551414197813 },
            { 3.006925247271267 },
            { 3.323443694352453 },
            { 3.673279872705343 },
            { 4.059940911937484 },
            { 4.487303113194061 },
            { 4.959650809319752 },
            { 5.481719315563936 },
            { 6.058742401412771 },
            { 6.696504759456221 },
            { 7.401399997293717 },
        };
        for(int i = 0; i < yResult.size(); i++) {
            assert(std::abs(tExpected[i] - tResult[i]) < EPS);
            for(int j = 0; j < yResult[i].size(); j++){
                assert(std::abs(yExpected[i][j] - yResult[i][j]) < EPS);
            }
        }
        return true;
    }


public:
    void run() override {
        TestOdeTrapez();
    }
};

int main() {
    ODETrapezTestCase().run();
    return 0;
}
