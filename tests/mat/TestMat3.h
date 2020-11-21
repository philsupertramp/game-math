#pragma once

#include "../Test.h"
#include <cassert>
#include <iostream>
#include <math/math.h> // NOLINT

template<class T>
class Mat3TestCase : public Test
{
    bool TestInitialization()
    {
        mat3<T> foo;
        assert(foo[0][0] == (T)0);
        assert(foo[0][1] == (T)0);
        assert(foo[0][2] == (T)0);
        assert(foo[1][0] == (T)0);

        return true;
    }
    bool TestUtils()
    {
        mat3<T> foo = mat3<T>::Unit();
        assert(foo[0][0] == (T)1);
        assert(foo[0][1] == (T)0);
        assert(foo[0][2] == (T)0);
        assert(foo[1][0] == (T)0);
        assert(foo[1][1] == (T)1);
        assert(foo[1][2] == (T)0);
        assert(foo[2][0] == (T)0);
        assert(foo[2][1] == (T)0);
        assert(foo[2][2] == (T)1);

        foo = mat3<T>((T)1, (T)2, (T)2, (T)2, (T)1, (T)2, (T)2, (T)2, (T)1);
        assert(foo[0][0] == (T)1);
        assert(foo[0][1] == (T)2);
        assert(foo[0][2] == (T)2);
        assert(foo[1][0] == (T)2);
        assert(foo[1][1] == (T)1);
        assert(foo[1][2] == (T)2);
        assert(foo[2][0] == (T)2);
        assert(foo[2][1] == (T)2);
        assert(foo[2][2] == (T)1);

        assert(foo.IsSymmetric());

        mat3<T> fooCopy = foo;
        assert(fooCopy[0][0] == foo[0][0]);
        assert(fooCopy[0][1] == foo[0][1]);
        assert(fooCopy[0][2] == foo[0][2]);
        assert(fooCopy[1][0] == foo[1][0]);
        assert(fooCopy[1][1] == foo[1][1]);
        assert(fooCopy[1][2] == foo[1][2]);
        assert(fooCopy[2][0] == foo[2][0]);
        assert(fooCopy[2][1] == foo[2][1]);
        assert(fooCopy[2][2] == foo[2][2]);

        fooCopy = fooCopy.Transpose();
        // nothing happened matrix is symmetric
        assert(fooCopy[0][0] == foo[0][0]);
        assert(fooCopy[0][1] == foo[0][1]);
        assert(fooCopy[0][2] == foo[0][2]);
        assert(fooCopy[1][0] == foo[1][0]);
        assert(fooCopy[1][1] == foo[1][1]);
        assert(fooCopy[1][2] == foo[1][2]);
        assert(fooCopy[2][0] == foo[2][0]);
        assert(fooCopy[2][1] == foo[2][1]);
        assert(fooCopy[2][2] == foo[2][2]);

        fooCopy = mat3<T>((T)0, (T)-1, (T)-1, (T)1, (T)0, (T)-1, (T)1, (T)1, (T)0);
        fooCopy = fooCopy.Transpose();
        assert(fooCopy[0][0] == (T)0);
        assert(fooCopy[0][1] == (T)1);
        assert(fooCopy[0][2] == (T)1);
        assert(fooCopy[1][0] == (T)-1);
        assert(fooCopy[1][1] == (T)0);
        assert(fooCopy[1][2] == (T)1);
        assert(fooCopy[2][0] == (T)-1);
        assert(fooCopy[2][1] == (T)-1);
        assert(fooCopy[2][2] == (T)0);

        std::cout << "mat3\n" << foo;

        return true;
    }

    bool TestMath()
    {
        mat3<T> foo(1, 2, 3, 4, 5, 6, 7, 8, 9);
        mat3<T> bar(10, 11, 12, 13, 14, 15, 16, 17, 18);
        vec3<T> vec(1, 2, 3);
        float delta = 2.0f;
        mat3<T> fooBar;

        // +
        fooBar = foo + bar;
        assert(fooBar[0][0] == (T)11.0f);
        assert(fooBar[0][1] == (T)13.0f);
        assert(fooBar[0][2] == (T)15.0f);
        assert(fooBar[1][0] == (T)17.0f);
        assert(fooBar[1][1] == (T)19.0f);
        assert(fooBar[1][2] == (T)21.0f);
        assert(fooBar[2][0] == (T)23.0f);
        assert(fooBar[2][1] == (T)25.0f);
        assert(fooBar[2][2] == (T)27.0f);

        fooBar = foo;
        fooBar += bar;
        assert(fooBar[0][0] == (T)11.0f);
        assert(fooBar[0][1] == (T)13.0f);
        assert(fooBar[0][2] == (T)15.0f);
        assert(fooBar[1][0] == (T)17.0f);
        assert(fooBar[1][1] == (T)19.0f);
        assert(fooBar[1][2] == (T)21.0f);
        assert(fooBar[2][0] == (T)23.0f);
        assert(fooBar[2][1] == (T)25.0f);
        assert(fooBar[2][2] == (T)27.0f);

        // -
        fooBar = foo - bar;
        assert(fooBar[0][0] == (T)-9.0f);
        assert(fooBar[0][1] == (T)-9.0f);
        assert(fooBar[0][2] == (T)-9.0f);
        assert(fooBar[1][0] == (T)-9.0f);
        assert(fooBar[1][1] == (T)-9.0f);
        assert(fooBar[1][2] == (T)-9.0f);
        assert(fooBar[2][0] == (T)-9.0f);
        assert(fooBar[2][1] == (T)-9.0f);
        assert(fooBar[2][2] == (T)-9.0f);
        fooBar = foo;
        fooBar -= bar;
        assert(fooBar[0][0] == (T)-9.0f);
        assert(fooBar[0][1] == (T)-9.0f);
        assert(fooBar[0][2] == (T)-9.0f);
        assert(fooBar[1][0] == (T)-9.0f);
        assert(fooBar[1][1] == (T)-9.0f);
        assert(fooBar[1][2] == (T)-9.0f);
        assert(fooBar[2][0] == (T)-9.0f);
        assert(fooBar[2][1] == (T)-9.0f);
        assert(fooBar[2][2] == (T)-9.0f);

        // /

        fooBar = foo / delta;
        assert(fooBar[0][0] == (T)0.5f);
        assert(fooBar[0][1] == (T)1.0f);
        assert(fooBar[0][2] == (T)1.5f);
        assert(fooBar[1][0] == (T)2.0f);
        assert(fooBar[1][1] == (T)2.5f);
        assert(fooBar[1][2] == (T)3.0f);
        assert(fooBar[2][0] == (T)3.5f);
        assert(fooBar[2][1] == (T)4.0f);
        assert(fooBar[2][2] == (T)4.5f);
        fooBar = foo;
        fooBar /= delta;
        assert(fooBar[0][0] == (T)0.5f);
        assert(fooBar[0][1] == (T)1.0f);
        assert(fooBar[0][2] == (T)1.5f);
        assert(fooBar[1][0] == (T)2.0f);
        assert(fooBar[1][1] == (T)2.5f);
        assert(fooBar[1][2] == (T)3.0f);
        assert(fooBar[2][0] == (T)3.5f);
        assert(fooBar[2][1] == (T)4.0f);
        assert(fooBar[2][2] == (T)4.5f);

        // *
        fooBar = foo * bar;
        assert(fooBar[0][0] == (T)84.0f);
        assert(fooBar[0][1] == (T)90.0f);
        assert(fooBar[0][2] == (T)96.0f);
        assert(fooBar[1][0] == (T)201.0f);
        assert(fooBar[1][1] == (T)216.0f);
        assert(fooBar[1][2] == (T)231.0f);
        assert(fooBar[2][0] == (T)318.0f);
        assert(fooBar[2][1] == (T)342.0f);
        assert(fooBar[2][2] == (T)366.0f);

        fooBar = foo;
        fooBar *= bar;
        assert(fooBar[0][0] == (T)84.0f);
        assert(fooBar[0][1] == (T)90.0f);
        assert(fooBar[0][2] == (T)96.0f);
        assert(fooBar[1][0] == (T)201.0f);
        assert(fooBar[1][1] == (T)216.0f);
        assert(fooBar[1][2] == (T)231.0f);
        assert(fooBar[2][0] == (T)318.0f);
        assert(fooBar[2][1] == (T)342.0f);
        assert(fooBar[2][2] == (T)366.0f);

        vec3<T> fooVec = foo * vec;
        assert(fooVec.x == (T)14.0f);
        assert(fooVec.y == (T)32.0f);
        assert(fooVec.z == (T)50.0f);

        fooBar = foo * delta;
        assert(fooBar[0][0] == (T)2.0f);
        assert(fooBar[0][1] == (T)4.0f);
        assert(fooBar[0][2] == (T)6.0f);
        assert(fooBar[1][0] == (T)8.0f);
        assert(fooBar[1][1] == (T)10.0f);
        assert(fooBar[1][2] == (T)12.0f);
        assert(fooBar[2][0] == (T)14.0f);
        assert(fooBar[2][1] == (T)16.0f);
        assert(fooBar[2][2] == (T)18.0f);
        fooBar = foo;
        fooBar *= delta;
        assert(fooBar[0][0] == (T)2.0f);
        assert(fooBar[0][1] == (T)4.0f);
        assert(fooBar[0][2] == (T)6.0f);
        assert(fooBar[1][0] == (T)8.0f);
        assert(fooBar[1][1] == (T)10.0f);
        assert(fooBar[1][2] == (T)12.0f);
        assert(fooBar[2][0] == (T)14.0f);
        assert(fooBar[2][1] == (T)16.0f);
        assert(fooBar[2][2] == (T)18.0f);

        return true;
    }

public:
    void run(bool _extended) override
    {
        extended = _extended;
        TestInitialization();
        TestUtils();
        TestMath();
    }
};

bool TestMat3()
{
    Mat3TestCase<int>().run(false);
    Mat3TestCase<float>().run(true);
    Mat3TestCase<double>().run(true);
    return true;
}
