#include "../Test.h"
#include <math/ds.h>


class PerceptronTestCase : public Test
{
    bool TestPerceptron() {
        auto ppn = Perceptron(0.1, 10);
        Matrix<double> A(
        { { 5.1, 1.4 }, { 4.9, 1.4 }, { 4.7, 1.3 }, { 4.6, 1.5 }, { 5.0, 1.4 }, { 5.4, 1.7 }, { 4.6, 1.4 },
          { 5.0, 1.5 }, { 4.4, 1.4 }, { 4.9, 1.5 }, { 5.4, 1.5 }, { 4.8, 1.6 }, { 4.8, 1.4 }, { 4.3, 1.1 },
          { 5.8, 1.2 }, { 5.7, 1.5 }, { 5.4, 1.3 }, { 5.1, 1.4 }, { 5.7, 1.7 }, { 5.1, 1.5 }, { 5.4, 1.7 },
          { 5.1, 1.5 }, { 4.6, 1.0 }, { 5.1, 1.7 }, { 4.8, 1.9 }, { 5.0, 1.6 }, { 5.0, 1.6 }, { 5.2, 1.5 },
          { 5.2, 1.4 }, { 4.7, 1.6 }, { 4.8, 1.6 }, { 5.4, 1.5 }, { 5.2, 1.5 }, { 5.5, 1.4 }, { 4.9, 1.5 },
          { 5.0, 1.2 }, { 5.5, 1.3 }, { 4.9, 1.5 }, { 4.4, 1.3 }, { 5.1, 1.5 }, { 5.0, 1.3 }, { 4.5, 1.3 },
          { 4.4, 1.3 }, { 5.0, 1.6 }, { 5.1, 1.9 }, { 4.8, 1.4 }, { 5.1, 1.6 }, { 4.6, 1.4 }, { 5.3, 1.5 },
          { 5.0, 1.4 }, { 7.0, 4.7 }, { 6.4, 4.5 }, { 6.9, 4.9 }, { 5.5, 4.0 }, { 6.5, 4.6 }, { 5.7, 4.5 },
          { 6.3, 4.7 }, { 4.9, 3.3 }, { 6.6, 4.6 }, { 5.2, 3.9 }, { 5.0, 3.5 }, { 5.9, 4.2 }, { 6.0, 4.0 },
          { 6.1, 4.7 }, { 5.6, 3.6 }, { 6.7, 4.4 }, { 5.6, 4.5 }, { 5.8, 4.1 }, { 6.2, 4.5 }, { 5.6, 3.9 },
          { 5.9, 4.8 }, { 6.1, 4.0 }, { 6.3, 4.9 }, { 6.1, 4.7 }, { 6.4, 4.3 }, { 6.6, 4.4 }, { 6.8, 4.8 },
          { 6.7, 5.0 }, { 6.0, 4.5 }, { 5.7, 3.5 }, { 5.5, 3.8 }, { 5.5, 3.7 }, { 5.8, 3.9 }, { 6.0, 5.1 },
          { 5.4, 4.5 }, { 6.0, 4.5 }, { 6.7, 4.7 }, { 6.3, 4.4 }, { 5.6, 4.1 }, { 5.5, 4.0 }, { 5.5, 4.4 },
          { 6.1, 4.6 }, { 5.8, 4.0 }, { 5.0, 3.3 }, { 5.6, 4.2 }, { 5.7, 4.2 }, { 5.7, 4.2 }, { 6.2, 4.3 },
          { 5.1, 3.0 }, { 5.7, 4.1 } });
        Matrix<double> B = {
            { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 },
            { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 },
            { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 },
            { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { 1 },  { 1 },
            { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },
            { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },
            { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },
            { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },  { 1 },
        };

        ppn.fit(A, B);
        assert(ppn.predict({ { 5.4, 0.2 } })(0, 0) == -1); // == -1
        //
        return true;
    }

public:
    void run() override { TestPerceptron(); }
};

int main() {
    PerceptronTestCase().run();
    return 0;
}