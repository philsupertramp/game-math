#include "../../Test.h"
#include <math/numerics.h>

class ODEBDF2TestCase : public Test
{
    bool TestOdeBDF2() {
        auto ode                     = []([[maybe_unused]] float t, std::vector<float> y) { return y; };
        std::vector<float> tInterval = { 0.0f, 2.0f };
        std::vector<float> y0        = { 1.0f };
        ODEOption options = {
            0.1f,
            1e-10,
            250,
            []([[maybe_unused]] float t, std::vector<float> y){ return std::vector<std::vector<float>>(y.size(), std::vector<float>(1)); }
        };

        auto foo = odeBDF2(ode, tInterval, y0, options);

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
            { 1.221804511278195 },
            { 1.350698174006445 },
            { 1.493210065981279 },
            { 1.650765032113812 },
            { 1.824946450883560 },
            { 2.017507418364439 },
            { 2.230386865205070 },
            { 2.465728586591371 },
            { 2.725902671843006 },
            { 3.013529321707376 },
            { 3.331505219638034 },
            { 3.683032698873129 },
            { 4.071651991376601 },
            { 4.501276880940456 },
            { 4.976234118709008 },
            { 5.501306997819849 },
            { 6.081783525917995 },
            { 6.723509680661476 },
            { 7.432948284545681 }
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
        TestOdeBDF2();
    }
};

int main() {
    ODEBDF2TestCase().run();
    return 0;
}
