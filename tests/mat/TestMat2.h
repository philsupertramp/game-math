#pragma once

#include "../Test.h"
#include <math/math.h> // NOLINT

template<class T>
class Mat2TestCase : public Test
{
    bool TestInitialization()
    {
        mat2<T> foo;
        assert(foo[0][0] == (T)0);
        assert(foo[0][1] == (T)0);
        assert(foo[1][0] == (T)0);
        assert(foo[1][1] == (T)0);

        return true;
    }
    bool TestUtils()
    {
        mat2<T> foo = mat2<T>::Unit();
        assert(foo[0][0] == (T)1);
        assert(foo[0][1] == (T)0);
        assert(foo[1][0] == (T)0);
        assert(foo[1][1] == (T)1);

        foo = mat2<T>((T)1, (T)2, (T)2, (T)1);
        assert(foo[0][0] == (T)1);
        assert(foo[0][1] == (T)2);
        assert(foo[1][0] == (T)2);
        assert(foo[1][1] == (T)1);

        assert(foo.IsSymmetric());

        mat2<T> fooCopy = foo;
        assert(fooCopy[0][0] == foo[0][0]);
        assert(fooCopy[0][1] == foo[0][1]);
        assert(fooCopy[1][0] == foo[1][0]);
        assert(fooCopy[1][1] == foo[1][1]);

        fooCopy = fooCopy.Transpose();
        // nothing happened matrix is symmetric
        assert(fooCopy[0][0] == foo[0][0]);
        assert(fooCopy[0][1] == foo[0][1]);
        assert(fooCopy[1][0] == foo[1][0]);
        assert(fooCopy[1][1] == foo[1][1]);
        fooCopy = mat2<T>((T)0, (T)-1, (T)1, (T)0);
        fooCopy = fooCopy.Transpose();
        assert(fooCopy[0][0] == (T)0);
        assert(fooCopy[0][1] == (T)1);
        assert(fooCopy[1][0] == (T)-1);
        assert(fooCopy[1][1] == (T)0);

        std::cout << "mat2\n" << foo;

        return true;
    }

    bool TestMath()
    {
        mat2<T> foo(1, 2, 3, 4);
        mat2<T> bar(5, 6, 7, 8);
        vec2<T> vec(1, 2);
        float delta = 2.0f;
        mat2<T> fooBar;

        // +
        fooBar = foo + bar;
        assert(fooBar[0][0] == (T)6.0f);
        assert(fooBar[0][1] == (T)8.0f);
        assert(fooBar[1][0] == (T)10.0f);
        assert(fooBar[1][1] == (T)12.0f);
        fooBar = foo;
        fooBar += bar;
        assert(fooBar[0][0] == (T)6.0f);
        assert(fooBar[0][1] == (T)8.0f);
        assert(fooBar[1][0] == (T)10.0f);
        assert(fooBar[1][1] == (T)12.0f);

        // -
        fooBar = foo - bar;
        assert(fooBar[0][0] == (T)-4.0f);
        assert(fooBar[0][1] == (T)-4.0f);
        assert(fooBar[1][0] == (T)-4.0f);
        assert(fooBar[1][1] == (T)-4.0f);
        fooBar = foo;
        fooBar -= bar;
        assert(fooBar[0][0] == (T)-4.0f);
        assert(fooBar[0][1] == (T)-4.0f);
        assert(fooBar[1][0] == (T)-4.0f);
        assert(fooBar[1][1] == (T)-4.0f);

        // /

        fooBar = foo / delta;
        assert(fooBar[0][0] == (T)0.5f);
        assert(fooBar[0][1] == (T)1.0f);
        assert(fooBar[1][0] == (T)1.5f);
        assert(fooBar[1][1] == (T)2.0f);
        fooBar = foo;
        fooBar /= delta;
        assert(fooBar[0][0] == (T)0.5f);
        assert(fooBar[0][1] == (T)1.0f);
        assert(fooBar[1][0] == (T)1.5f);
        assert(fooBar[1][1] == (T)2.0f);

        // *
        fooBar = foo * bar;
        assert(fooBar[0][0] == (T)19.0f);
        assert(fooBar[0][1] == (T)22.0f);
        assert(fooBar[1][0] == (T)43.0f);
        assert(fooBar[1][1] == (T)50.0f);

        fooBar = foo;
        fooBar *= bar;
        assert(fooBar[0][0] == (T)19.0f);
        assert(fooBar[0][1] == (T)22.0f);
        assert(fooBar[1][0] == (T)43.0f);
        assert(fooBar[1][1] == (T)50.0f);

        vec2<T> fooVec = foo * vec;
        assert(fooVec.x == (T)5.0f);
        assert(fooVec.y == (T)11.0f);

        fooBar = foo * delta;
        assert(fooBar[0][0] == (T)2.0f);
        assert(fooBar[0][1] == (T)4.0f);
        assert(fooBar[1][0] == (T)6.0f);
        assert(fooBar[1][1] == (T)8.0f);
        fooBar = foo;
        fooBar *= delta;
        assert(fooBar[0][0] == (T)2.0f);
        assert(fooBar[0][1] == (T)4.0f);
        assert(fooBar[1][0] == (T)6.0f);
        assert(fooBar[1][1] == (T)8.0f);

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

bool TestMat2()
{
    Mat2TestCase<int>().run(false);
    Mat2TestCase<float>().run(true);
    Mat2TestCase<double>().run(true);
    return true;
}
