#include "newton.h"
#include <functional>

std::vector<float> newton(
const std::function<std::vector<float>(std::vector<float>)>& f,
const std::function<std::vector<std::vector<float>>(std::vector<float>)>& Df, const std::vector<float>& x0, float TOL, int maxIter){
    auto m = x0.size();
    if(m < 1){
        // error
    }
    std::vector<float> x = x0;

    int iter = 0;
    double r = TOL + 1;

    while(r > TOL && iter < maxIter){
        auto D_F = Df(x);
        auto F = f(x);

        for(float & i : F) {
            i = -i;
        }

        auto delta = gaussSeidel(D_F, F);
        for(int i = 0; i < x.size(); ++i) {
            x[i] += delta[i];
        }

        r = norm({ x });
        iter += 1;
    }
    if(iter == maxIter){
        // error max Iter reached
    }
    return x;
}