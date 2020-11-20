#pragma once

#include "../Test.h"
#include <cassert>
#include <iostream>
#include <math/math.h> // NOLINT


template<class T>
class Vec3TestCase : public Test
{
    bool extended = false;

    bool testInitialization()
    {
        vec3<T> foo;
        assert(foo.x == (T)0);
        assert(foo.y == (T)0);
        assert(foo.z == (T)0);

        vec3<T> foo2((T)1, (T)2, (T)3);
        assert(foo2.x == (T)1);
        assert(foo2.y == (T)2);
        assert(foo2.z == (T)3);

        T vals[3]   = { 2, 4, 6 };
        vec3<T> vec = build_vec3<T>((void*)vals);
        assert(vec.x == (T)2);
        assert(vec.y == (T)4);
        assert(vec.z == (T)6);

        return true;
    }
    bool testAssignment()
    {
        vec3<T> foo;

        foo.x = (T)1;
        foo.y = (T)2;
        foo.z = (T)3;

        assert(foo.x == (T)1);
        assert(foo.y == (T)2);
        assert(foo.z == (T)3);

        foo[0] = (T)4;
        foo[1] = (T)5;
        foo[2] = (T)6;

        assert(foo.x == (T)4);
        assert(foo.y == (T)5);
        assert(foo.z == (T)6);
        return true;
    }
    bool testMemberAccess()
    {
        vec3<T> foo;

        foo.x = (T)1;
        foo.y = (T)2;
        foo.z = (T)3;

        assert(foo[0] == 1);
        assert(foo[1] == 2);
        assert(foo[2] == 3);
        return true;
    }
    bool testMisc()
    {
        vec3<T> foo(2, 2, 2);
        vec3<T> bar(2, -2, 2);
        vec3<T> baz(10, 0, 2);

        assert(foo.length() == 3.46410155f);
        assert(bar.length() == 3.46410155f);
        assert(baz.length() == 10.1980391f);

        assert(foo.cross(foo) == vec3<T>());
        assert(foo.cross(bar) == vec3<T>(8, 0, -8));
        assert(foo.cross(baz) == vec3<T>(4, 16, -20));
        assert(bar.cross(foo) == vec3<T>(-8, 0, 8));
        assert(bar.cross(baz) == vec3<T>(-4, 16, 20));
        assert(baz.cross(foo) == vec3<T>(-4, -16, 20));
        assert(baz.cross(bar) == vec3<T>(4, -16, -20));

        if(extended)
        {
            vec3<T> normFoo = foo.normalize();
            vec3<T> normBar = bar.normalize();
            vec3<T> normBaz = baz.normalize();

            // floating point precision 1e-7
            assert(normFoo.x - (T)0.577350259f < 1e-7);
            assert(normFoo.y - (T)0.577350259f < 1e-7);
            assert(normFoo.z - (T)0.577350259f < 1e-7);
            assert(normBar.x - (T)0.577350259f < 1e-7);
            assert(normBar.y - (T)-0.577350259f < 1e-7);
            assert(normBar.z - (T)0.577350259f < 1e-7);
            assert(normBaz.x - (T)0.980580688f < 1e-7);
            assert(normBaz.y - (T)0.0f < 1e-7);
            assert(normBaz.z - (T)0.196116135 < 1e-7);
        }
        return true;
    }
    bool testOperators()
    {
        vec3<T> foo(1, 1, 1);
        vec3<T> bar(1, -1, 1);
        vec3<T> baz(10, 0, 0);
        // +
        vec3<T> fooCopy = foo;
        fooCopy += bar;
        assert(fooCopy.x == 2.0f);
        assert(fooCopy.y == 0.0f);
        assert(fooCopy.z == 2.0f);
        fooCopy = foo + bar;
        assert(fooCopy.x == 2.0f);
        assert(fooCopy.y == 0.0f);
        assert(fooCopy.z == 2.0f);
        fooCopy = foo;

        // -
        fooCopy -= bar;
        assert(fooCopy.x == 0.0f);
        assert(fooCopy.y == 2.0f);
        assert(fooCopy.z == 0.0f);
        fooCopy = foo - bar;
        assert(fooCopy.x == 0.0f);
        assert(fooCopy.y == 2.0f);
        assert(fooCopy.z == 0.0f);
        fooCopy = foo;

        // *
        fooCopy *= 2.0f;
        assert(fooCopy.x == (T)2.0f);
        assert(fooCopy.y == (T)2.0f);
        assert(fooCopy.z == (T)2.0f);
        fooCopy = foo * 2.0f;
        assert(fooCopy.x == (T)2.0f);
        assert(fooCopy.y == (T)2.0f);
        assert(fooCopy.z == (T)2.0f);

        float fooVal = foo * bar;
        assert(fooVal == (T)1.0f);
        fooVal = foo * baz;
        assert(fooVal == (T)10.0f);
        fooCopy = foo;

        // /
        fooCopy /= 2.0f;
        assert(fooCopy.x == (T)0.5f);
        assert(fooCopy.y == (T)0.5f);
        assert(fooCopy.z == (T)0.5f);
        fooCopy = foo / 2.0f;
        assert(fooCopy.x == (T)0.5f);
        assert(fooCopy.y == (T)0.5f);
        assert(fooCopy.z == (T)0.5f);

        // ostream<<
        std::cout << "vec3:\n" << foo;

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

bool TestVec3()
{
    Vec3TestCase<int>().run();
    Vec3TestCase<float>().run(true);
    Vec3TestCase<double>().run(true);

    return true;
}
