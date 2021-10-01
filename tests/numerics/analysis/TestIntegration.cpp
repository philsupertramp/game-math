#include "../../Test.h"
#include <math/numerics/analysis/Integration.h>


class IntegrationTestCase : public Test
{

  bool TestQuadAdaptive(){

    auto res = quad_adaptive(sin, 0, 3.14, 0.6, 1.e-30);

    std::cout << res.first << std::endl;
    return true;
  }

public:
    virtual void run() { TestQuadAdaptive();  }
};


int main() {
    IntegrationTestCase().run();
    return 0;
}

