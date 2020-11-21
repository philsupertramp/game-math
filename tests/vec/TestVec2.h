#pragma once

#include "../Test.h"
#include <cassert>
#include <iostream>
#include <math/math.h>


template<class T>
class Vec2TestCase : public Test
{
    bool testInitialization()
    {
        vec2<T> foo;
        assert(foo.x == (T)0);
        assert(foo.y == (T)0);

        vec2<T> foo2((T)1, (T)2);
        assert(foo2.x == (T)1);
        assert(foo2.y == (T)2);

        T vals[2]   = { 2, 4 };
        vec2<T> vec = build_vec2<T>((void*)vals);
        assert(vec.x == (T)2);
        assert(vec.y == (T)4);

        return true;
    }
    bool testAssignment()
    {
        vec2<T> foo;

        foo.x = (T)1;
        foo.y = (T)2;

        assert(foo.x == (T)1);
        assert(foo.y == (T)2);

        foo[0] = (T)3;
        foo[1] = (T)4;

        assert(foo.x == (T)3);
        assert(foo.y == (T)4);
        return true;
    }
    bool testMemberAccess()
    {
        vec2<T> foo;

        foo.x = (T)1;
        foo.y = (T)2;

        assert(foo[0] == 1);
        assert(foo[1] == 2);
        return true;
    }
    bool testMisc()
    {
        vec2<T> foo(2, 2);
        vec2<T> bar(2, -2);
        vec2<T> baz(10, 0);

        assert(foo.length() == 2.82842708f);
        assert(bar.length() == 2.82842708f);
        assert(baz.length() == 10.0f);

        if(extended)
        {
            vec2<T> normFoo = foo.normalize();
            vec2<T> normBar = bar.normalize();
            vec2<T> normBaz = baz.normalize();

            // floating point precision 1e-7
            assert(normFoo.x - (T)0.70710679328816484f < 1e-7);
            assert(normFoo.y - (T)0.70710679328816484f < 1e-7);
            assert(normBar.x - (T)0.70710679328816484f < 1e-7);
            assert(normBar.y - (T)-0.70710679328816484f < 1e-7);
            assert(normBaz.x - (T)1.0f < 1e-7);
            assert(normBaz.y - (T)0.0f < 1e-7);
        }
        return true;
    }
    bool testOperators()
    {
        vec2<T> foo(1, 1);
        vec2<T> bar(1, -1);
        vec2<T> baz(10, 0);
        // +
        vec2<T> fooCopy = foo;
        fooCopy += bar;
        assert(fooCopy.x == 2.0f);
        assert(fooCopy.y == 0.0f);
        fooCopy = foo + bar;
        assert(fooCopy.x == 2.0f);
        assert(fooCopy.y == 0.0f);
        fooCopy = foo;

        // -
        fooCopy -= bar;
        assert(fooCopy.x == 0.0f);
        assert(fooCopy.y == 2.0f);
        fooCopy = foo - bar;
        assert(fooCopy.x == 0.0f);
        assert(fooCopy.y == 2.0f);
        fooCopy = foo;

        // *
        fooCopy *= 2.0f;
        assert(fooCopy.x == (T)2.0f);
        assert(fooCopy.y == (T)2.0f);
        fooCopy = foo * 2.0f;
        assert(fooCopy.x == (T)2.0f);
        assert(fooCopy.y == (T)2.0f);

        float fooVal = foo * bar;
        assert(fooVal == (T)0.0f);
        fooVal = foo * baz;
        assert(fooVal == (T)10.0f);
        fooCopy = foo;

        // /
        fooCopy /= 2.0f;
        assert(fooCopy.x == (T)0.5f);
        assert(fooCopy.y == (T)0.5f);
        fooCopy = foo / 2.0f;
        assert(fooCopy.x == (T)0.5f);
        assert(fooCopy.y == (T)0.5f);

        // ostream<<
        std::cout << "vec2:\n" << foo;

        return true;
    }

public:
    virtual void run(bool _extended = false) override
    {
        extended = _extended;

        testInitialization();
        testAssignment();
        testMemberAccess();
        testMisc();
        testOperators();
    }
};

bool TestVec2()
{
    Vec2TestCase<int>().run();
    Vec2TestCase<float>().run(true);
    Vec2TestCase<double>().run(true);

    return true;
}
