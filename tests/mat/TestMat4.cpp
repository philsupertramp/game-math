#include "../Test.h"
#include <cassert>
#include <iostream>
#include <math/math.h> // NOLINT

template<class T>
class Mat4TestCase : public Test
{
    bool TestInitialization() {
        mat4<T> foo;
        assert(foo[0][0] == (T)0);
        assert(foo[0][1] == (T)0);
        assert(foo[0][2] == (T)0);
        assert(foo[0][3] == (T)0);
        assert(foo[1][0] == (T)0);
        assert(foo[1][1] == (T)0);
        assert(foo[1][2] == (T)0);
        assert(foo[1][3] == (T)0);
        assert(foo[2][0] == (T)0);
        assert(foo[2][1] == (T)0);
        assert(foo[2][2] == (T)0);
        assert(foo[2][3] == (T)0);
        assert(foo[3][0] == (T)0);
        assert(foo[3][1] == (T)0);
        assert(foo[3][2] == (T)0);
        assert(foo[3][3] == (T)0);

        return true;
    }
    bool TestUtils() {
        mat4<T> foo = mat4<T>::Unit();
        assert(foo[0][0] == (T)1);
        assert(foo[0][1] == (T)0);
        assert(foo[0][2] == (T)0);
        assert(foo[0][3] == (T)0);
        assert(foo[1][0] == (T)0);
        assert(foo[1][1] == (T)1);
        assert(foo[1][2] == (T)0);
        assert(foo[1][3] == (T)0);
        assert(foo[2][0] == (T)0);
        assert(foo[2][1] == (T)0);
        assert(foo[2][2] == (T)1);
        assert(foo[2][3] == (T)0);
        assert(foo[3][0] == (T)0);
        assert(foo[3][1] == (T)0);
        assert(foo[3][2] == (T)0);
        assert(foo[3][3] == (T)1);

        foo = mat4<T>((T)1, (T)2, (T)2, (T)2, (T)2, (T)1, (T)2, (T)2, (T)2, (T)2, (T)1, (T)2, (T)2, (T)2, (T)2, (T)1);
        assert(foo[0][0] == (T)1);
        assert(foo[0][1] == (T)2);
        assert(foo[0][2] == (T)2);
        assert(foo[0][3] == (T)2);
        assert(foo[1][0] == (T)2);
        assert(foo[1][1] == (T)1);
        assert(foo[1][2] == (T)2);
        assert(foo[1][3] == (T)2);
        assert(foo[2][0] == (T)2);
        assert(foo[2][1] == (T)2);
        assert(foo[2][2] == (T)1);
        assert(foo[2][3] == (T)2);
        assert(foo[3][0] == (T)2);
        assert(foo[3][1] == (T)2);
        assert(foo[3][2] == (T)2);
        assert(foo[3][3] == (T)1);

        assert(foo.IsSymmetric());

        mat4<T> fooCopy = foo;
        assert(fooCopy[0][0] == foo[0][0]);
        assert(fooCopy[0][1] == foo[0][1]);
        assert(fooCopy[0][2] == foo[0][2]);
        assert(fooCopy[0][3] == foo[0][3]);
        assert(fooCopy[1][0] == foo[1][0]);
        assert(fooCopy[1][1] == foo[1][1]);
        assert(fooCopy[1][2] == foo[1][2]);
        assert(fooCopy[1][3] == foo[1][3]);
        assert(fooCopy[2][0] == foo[2][0]);
        assert(fooCopy[2][1] == foo[2][1]);
        assert(fooCopy[2][2] == foo[2][2]);
        assert(fooCopy[2][3] == foo[2][3]);
        assert(fooCopy[3][0] == foo[3][0]);
        assert(fooCopy[3][1] == foo[3][1]);
        assert(fooCopy[3][2] == foo[3][2]);
        assert(fooCopy[3][3] == foo[3][3]);

        fooCopy = fooCopy.Transpose();
        // nothing happened matrix is symmetric
        assert(fooCopy[0][0] == foo[0][0]);
        assert(fooCopy[0][1] == foo[0][1]);
        assert(fooCopy[0][2] == foo[0][2]);
        assert(fooCopy[0][3] == foo[0][3]);
        assert(fooCopy[1][0] == foo[1][0]);
        assert(fooCopy[1][1] == foo[1][1]);
        assert(fooCopy[1][2] == foo[1][2]);
        assert(fooCopy[1][3] == foo[1][3]);
        assert(fooCopy[2][0] == foo[2][0]);
        assert(fooCopy[2][1] == foo[2][1]);
        assert(fooCopy[2][2] == foo[2][2]);
        assert(fooCopy[2][3] == foo[2][3]);
        assert(fooCopy[3][0] == foo[3][0]);
        assert(fooCopy[3][1] == foo[3][1]);
        assert(fooCopy[3][2] == foo[3][2]);
        assert(fooCopy[3][3] == foo[3][3]);

        fooCopy =
        mat4<T>((T)0, (T)1, (T)1, (T)1, (T)-1, (T)0, (T)1, (T)1, (T)-1, (T)-1, (T)0, (T)1, (T)-1, (T)-1, (T)-1, (T)0);
        fooCopy = fooCopy.Transpose();
        assert(fooCopy[0][0] == (T)0);
        assert(fooCopy[0][1] == (T)-1);
        assert(fooCopy[0][2] == (T)-1);
        assert(fooCopy[0][3] == (T)-1);
        assert(fooCopy[1][0] == (T)1);
        assert(fooCopy[1][1] == (T)0);
        assert(fooCopy[1][2] == (T)-1);
        assert(fooCopy[1][3] == (T)-1);
        assert(fooCopy[2][0] == (T)1);
        assert(fooCopy[2][1] == (T)1);
        assert(fooCopy[2][2] == (T)0);
        assert(fooCopy[2][3] == (T)-1);
        assert(fooCopy[3][0] == (T)1);
        assert(fooCopy[3][1] == (T)1);
        assert(fooCopy[3][2] == (T)1);
        assert(fooCopy[3][3] == (T)0);

        std::cout << "mat4\n" << foo;

        return true;
    }

    bool TestMath() {
        mat4<T> foo(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        mat4<T> bar(17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
        vec4<T> vec(1, 2, 3, 4);
        float delta = 2.0f;
        mat4<T> fooBar;

        // +
        fooBar = foo + bar;
        assert(fooBar[0][0] == (T)18.0f);
        assert(fooBar[0][1] == (T)20.0f);
        assert(fooBar[0][2] == (T)22.0f);
        assert(fooBar[0][3] == (T)24.0f);
        assert(fooBar[1][0] == (T)26.0f);
        assert(fooBar[1][1] == (T)28.0f);
        assert(fooBar[1][2] == (T)30.0f);
        assert(fooBar[1][3] == (T)32.0f);
        assert(fooBar[2][0] == (T)34.0f);
        assert(fooBar[2][1] == (T)36.0f);
        assert(fooBar[2][2] == (T)38.0f);
        assert(fooBar[2][3] == (T)40.0f);
        assert(fooBar[3][0] == (T)42.0f);
        assert(fooBar[3][1] == (T)44.0f);
        assert(fooBar[3][2] == (T)46.0f);
        assert(fooBar[3][3] == (T)48.0f);

        fooBar = foo;
        fooBar += bar;
        assert(fooBar[0][0] == (T)18.0f);
        assert(fooBar[0][1] == (T)20.0f);
        assert(fooBar[0][2] == (T)22.0f);
        assert(fooBar[0][3] == (T)24.0f);
        assert(fooBar[1][0] == (T)26.0f);
        assert(fooBar[1][1] == (T)28.0f);
        assert(fooBar[1][2] == (T)30.0f);
        assert(fooBar[1][3] == (T)32.0f);
        assert(fooBar[2][0] == (T)34.0f);
        assert(fooBar[2][1] == (T)36.0f);
        assert(fooBar[2][2] == (T)38.0f);
        assert(fooBar[2][3] == (T)40.0f);
        assert(fooBar[3][0] == (T)42.0f);
        assert(fooBar[3][1] == (T)44.0f);
        assert(fooBar[3][2] == (T)46.0f);
        assert(fooBar[3][3] == (T)48.0f);

        // -
        fooBar = foo - bar;
        assert(fooBar[0][0] == (T)-16.0f);
        assert(fooBar[0][1] == (T)-16.0f);
        assert(fooBar[0][2] == (T)-16.0f);
        assert(fooBar[0][3] == (T)-16.0f);
        assert(fooBar[1][0] == (T)-16.0f);
        assert(fooBar[1][1] == (T)-16.0f);
        assert(fooBar[1][2] == (T)-16.0f);
        assert(fooBar[1][3] == (T)-16.0f);
        assert(fooBar[2][0] == (T)-16.0f);
        assert(fooBar[2][1] == (T)-16.0f);
        assert(fooBar[2][2] == (T)-16.0f);
        assert(fooBar[2][3] == (T)-16.0f);
        assert(fooBar[3][0] == (T)-16.0f);
        assert(fooBar[3][1] == (T)-16.0f);
        assert(fooBar[3][2] == (T)-16.0f);
        assert(fooBar[3][3] == (T)-16.0f);

        fooBar = foo;
        fooBar -= bar;
        assert(fooBar[0][0] == (T)-16.0f);
        assert(fooBar[0][1] == (T)-16.0f);
        assert(fooBar[0][2] == (T)-16.0f);
        assert(fooBar[0][3] == (T)-16.0f);
        assert(fooBar[1][0] == (T)-16.0f);
        assert(fooBar[1][1] == (T)-16.0f);
        assert(fooBar[1][2] == (T)-16.0f);
        assert(fooBar[1][3] == (T)-16.0f);
        assert(fooBar[2][0] == (T)-16.0f);
        assert(fooBar[2][1] == (T)-16.0f);
        assert(fooBar[2][2] == (T)-16.0f);
        assert(fooBar[2][3] == (T)-16.0f);
        assert(fooBar[3][0] == (T)-16.0f);
        assert(fooBar[3][1] == (T)-16.0f);
        assert(fooBar[3][2] == (T)-16.0f);
        assert(fooBar[3][3] == (T)-16.0f);


        // /

        fooBar = foo / delta;
        assert(fooBar[0][0] == (T)0.5f);
        assert(fooBar[0][1] == (T)1.0f);
        assert(fooBar[0][2] == (T)1.5f);
        assert(fooBar[0][3] == (T)2.0f);
        assert(fooBar[1][0] == (T)2.5f);
        assert(fooBar[1][1] == (T)3.0f);
        assert(fooBar[1][2] == (T)3.5f);
        assert(fooBar[1][3] == (T)4.0f);
        assert(fooBar[2][0] == (T)4.5f);
        assert(fooBar[2][1] == (T)5.0f);
        assert(fooBar[2][2] == (T)5.5f);
        assert(fooBar[2][3] == (T)6.0f);
        assert(fooBar[3][0] == (T)6.5f);
        assert(fooBar[3][1] == (T)7.0f);
        assert(fooBar[3][2] == (T)7.5f);
        assert(fooBar[3][3] == (T)8.0f);
        fooBar = foo;
        fooBar /= delta;
        assert(fooBar[0][0] == (T)0.5f);
        assert(fooBar[0][1] == (T)1.0f);
        assert(fooBar[0][2] == (T)1.5f);
        assert(fooBar[0][3] == (T)2.0f);
        assert(fooBar[1][0] == (T)2.5f);
        assert(fooBar[1][1] == (T)3.0f);
        assert(fooBar[1][2] == (T)3.5f);
        assert(fooBar[1][3] == (T)4.0f);
        assert(fooBar[2][0] == (T)4.5f);
        assert(fooBar[2][1] == (T)5.0f);
        assert(fooBar[2][2] == (T)5.5f);
        assert(fooBar[2][3] == (T)6.0f);
        assert(fooBar[3][0] == (T)6.5f);
        assert(fooBar[3][1] == (T)7.0f);
        assert(fooBar[3][2] == (T)7.5f);
        assert(fooBar[3][3] == (T)8.0f);

        // *
        fooBar = foo * bar;
        assert(fooBar[0][0] == (T)250.0f);
        assert(fooBar[0][1] == (T)260.0f);
        assert(fooBar[0][2] == (T)270.0f);
        assert(fooBar[0][3] == (T)280.0f);
        assert(fooBar[1][0] == (T)618.0f);
        assert(fooBar[1][1] == (T)644.0f);
        assert(fooBar[1][2] == (T)670.0f);
        assert(fooBar[1][3] == (T)696.0f);
        assert(fooBar[2][0] == (T)986.0f);
        assert(fooBar[2][1] == (T)1028.0f);
        assert(fooBar[2][2] == (T)1070.0f);
        assert(fooBar[2][3] == (T)1112.0f);
        assert(fooBar[3][0] == (T)1354.0f);
        assert(fooBar[3][1] == (T)1412.0f);
        assert(fooBar[3][2] == (T)1470.0f);
        assert(fooBar[3][3] == (T)1528.0f);

        fooBar = foo;
        fooBar *= bar;
        assert(fooBar[0][0] == (T)250.0f);
        assert(fooBar[0][1] == (T)260.0f);
        assert(fooBar[0][2] == (T)270.0f);
        assert(fooBar[0][3] == (T)280.0f);
        assert(fooBar[1][0] == (T)618.0f);
        assert(fooBar[1][1] == (T)644.0f);
        assert(fooBar[1][2] == (T)670.0f);
        assert(fooBar[1][3] == (T)696.0f);
        assert(fooBar[2][0] == (T)986.0f);
        assert(fooBar[2][1] == (T)1028.0f);
        assert(fooBar[2][2] == (T)1070.0f);
        assert(fooBar[2][3] == (T)1112.0f);
        assert(fooBar[3][0] == (T)1354.0f);
        assert(fooBar[3][1] == (T)1412.0f);
        assert(fooBar[3][2] == (T)1470.0f);
        assert(fooBar[3][3] == (T)1528.0f);

        vec4<T> fooVec = foo * vec;
        assert(fooVec.x == (T)30.0f);
        assert(fooVec.y == (T)70.0f);
        assert(fooVec.z == (T)110.0f);
        assert(fooVec.w == (T)150.0f);

        fooBar = foo * delta;
        assert(fooBar[0][0] == (T)2.0f);
        assert(fooBar[0][1] == (T)4.0f);
        assert(fooBar[0][2] == (T)6.0f);
        assert(fooBar[0][3] == (T)8.0f);
        assert(fooBar[1][0] == (T)10.0f);
        assert(fooBar[1][1] == (T)12.0f);
        assert(fooBar[1][2] == (T)14.0f);
        assert(fooBar[1][3] == (T)16.0f);
        assert(fooBar[2][0] == (T)18.0f);
        assert(fooBar[2][1] == (T)20.0f);
        assert(fooBar[2][2] == (T)22.0f);
        assert(fooBar[2][3] == (T)24.0f);
        assert(fooBar[3][0] == (T)26.0f);
        assert(fooBar[3][1] == (T)28.0f);
        assert(fooBar[3][2] == (T)30.0f);
        assert(fooBar[3][3] == (T)32.0f);
        fooBar = foo;
        fooBar *= delta;
        assert(fooBar[0][0] == (T)2.0f);
        assert(fooBar[0][1] == (T)4.0f);
        assert(fooBar[0][2] == (T)6.0f);
        assert(fooBar[0][3] == (T)8.0f);
        assert(fooBar[1][0] == (T)10.0f);
        assert(fooBar[1][1] == (T)12.0f);
        assert(fooBar[1][2] == (T)14.0f);
        assert(fooBar[1][3] == (T)16.0f);
        assert(fooBar[2][0] == (T)18.0f);
        assert(fooBar[2][1] == (T)20.0f);
        assert(fooBar[2][2] == (T)22.0f);
        assert(fooBar[2][3] == (T)24.0f);
        assert(fooBar[3][0] == (T)26.0f);
        assert(fooBar[3][1] == (T)28.0f);
        assert(fooBar[3][2] == (T)30.0f);
        assert(fooBar[3][3] == (T)32.0f);

        return true;
    }

public:
    void run(bool _extended) override {
        extended = _extended;
        TestInitialization();
        TestUtils();
        TestMath();
    }
};

int main() {
    Mat4TestCase<int>().run(false);
    Mat4TestCase<float>().run(true);
    Mat4TestCase<double>().run(true);
    return 0;
}
