#pragma once

#include <cassert>
#include <math/math.h>  // NOLINT
#include "../Test.h"

template<class T>
class Mat3TestCase
: public Test
{
    bool TestInitialization() {
        mat3<T> foo;
        assert(foo.a == (T) 0);
        assert(foo.b == (T) 0);
        assert(foo.c == (T) 0);
        assert(foo.d == (T) 0);

        return true;
    }
    bool TestUtils(){
        mat3<T> foo = mat3<T>::Unit();
        assert(foo.a == (T)1);
        assert(foo.b == (T)0);
        assert(foo.c == (T)0);
        assert(foo.d == (T)0);
        assert(foo.e == (T)1);
        assert(foo.f == (T)0);
        assert(foo.g == (T)0);
        assert(foo.h == (T)0);
        assert(foo.i == (T)1);

        foo = mat3<T>((T) 1, (T) 2, (T) 2, (T) 2,(T)1,(T)2,(T)2,(T)2,(T)1);
        assert(foo.a == (T)1);
        assert(foo.b == (T)2);
        assert(foo.c == (T)2);
        assert(foo.d == (T)2);
        assert(foo.e == (T)1);
        assert(foo.f == (T)2);
        assert(foo.g == (T)2);
        assert(foo.h == (T)2);
        assert(foo.i == (T)1);

        assert(foo.IsSymmetric());

        mat3<T> fooCopy = foo;
        assert(fooCopy.a == foo.a);
        assert(fooCopy.b == foo.b);
        assert(fooCopy.c == foo.c);
        assert(fooCopy.d == foo.d);
        assert(fooCopy.e == foo.e);
        assert(fooCopy.f == foo.f);
        assert(fooCopy.g == foo.g);
        assert(fooCopy.h == foo.h);
        assert(fooCopy.i == foo.i);

        fooCopy = fooCopy.Transpose();
        // nothing happened matrix is symmetric
        assert(fooCopy.a == foo.a);
        assert(fooCopy.b == foo.b);
        assert(fooCopy.c == foo.c);
        assert(fooCopy.d == foo.d);
        assert(fooCopy.e == foo.e);
        assert(fooCopy.f == foo.f);
        assert(fooCopy.g == foo.g);
        assert(fooCopy.h == foo.h);
        assert(fooCopy.i == foo.i);

        fooCopy = mat3<T>((T)0,(T)-1,(T)-1,(T)1,(T)0,(T)-1,(T)1,(T)1,(T)0);
        fooCopy = fooCopy.Transpose();
        assert(fooCopy.a == (T) 0);
        assert(fooCopy.b == (T) 1);
        assert(fooCopy.c == (T) 1);
        assert(fooCopy.d == (T)-1);
        assert(fooCopy.e == (T) 0);
        assert(fooCopy.f == (T) 1);
        assert(fooCopy.g == (T)-1);
        assert(fooCopy.h == (T)-1);
        assert(fooCopy.i == (T) 0);

        std::cout << "mat3\n" << foo;

        return true;
    }

    bool TestMath(){
        mat3<T> foo(1,2,3,4,5,6,7,8,9);
        mat3<T> bar(10,11,12,13,14,15,16,17,18);
        vec3<T> vec(1,2,3);
        float delta = 2.0f;
        mat3<T> fooBar;

        // +
        fooBar = foo + bar;
        assert(fooBar.a == (T)11.0f);
        assert(fooBar.b == (T)13.0f);
        assert(fooBar.c == (T)15.0f);
        assert(fooBar.d == (T)17.0f);
        assert(fooBar.e == (T)19.0f);
        assert(fooBar.f == (T)21.0f);
        assert(fooBar.g == (T)23.0f);
        assert(fooBar.h == (T)25.0f);
        assert(fooBar.i == (T)27.0f);

        fooBar = foo;
        fooBar += bar;
        assert(fooBar.a == (T)11.0f);
        assert(fooBar.b == (T)13.0f);
        assert(fooBar.c == (T)15.0f);
        assert(fooBar.d == (T)17.0f);
        assert(fooBar.e == (T)19.0f);
        assert(fooBar.f == (T)21.0f);
        assert(fooBar.g == (T)23.0f);
        assert(fooBar.h == (T)25.0f);
        assert(fooBar.i == (T)27.0f);

        // -
        fooBar = foo - bar;
        assert(fooBar.a == (T)-9.0f);
        assert(fooBar.b == (T)-9.0f);
        assert(fooBar.c == (T)-9.0f);
        assert(fooBar.d == (T)-9.0f);
        assert(fooBar.e == (T)-9.0f);
        assert(fooBar.f == (T)-9.0f);
        assert(fooBar.g == (T)-9.0f);
        assert(fooBar.h == (T)-9.0f);
        assert(fooBar.i == (T)-9.0f);
        fooBar = foo;
        fooBar -= bar;
        assert(fooBar.a == (T)-9.0f);
        assert(fooBar.b == (T)-9.0f);
        assert(fooBar.c == (T)-9.0f);
        assert(fooBar.d == (T)-9.0f);
        assert(fooBar.e == (T)-9.0f);
        assert(fooBar.f == (T)-9.0f);
        assert(fooBar.g == (T)-9.0f);
        assert(fooBar.h == (T)-9.0f);
        assert(fooBar.i == (T)-9.0f);

        // /

        fooBar = foo / delta;
        assert(fooBar.a == (T)0.5f);
        assert(fooBar.b == (T)1.0f);
        assert(fooBar.c == (T)1.5f);
        assert(fooBar.d == (T)2.0f);
        assert(fooBar.e == (T)2.5f);
        assert(fooBar.f == (T)3.0f);
        assert(fooBar.g == (T)3.5f);
        assert(fooBar.h == (T)4.0f);
        assert(fooBar.i == (T)4.5f);
        fooBar = foo;
        fooBar /= delta;
        assert(fooBar.a == (T)0.5f);
        assert(fooBar.b == (T)1.0f);
        assert(fooBar.c == (T)1.5f);
        assert(fooBar.d == (T)2.0f);
        assert(fooBar.e == (T)2.5f);
        assert(fooBar.f == (T)3.0f);
        assert(fooBar.g == (T)3.5f);
        assert(fooBar.h == (T)4.0f);
        assert(fooBar.i == (T)4.5f);

        // *
        fooBar = foo * bar;
        assert(fooBar.a == (T)84.0f);
        assert(fooBar.b == (T)90.0f);
        assert(fooBar.c == (T)96.0f);
        assert(fooBar.d == (T)201.0f);
        assert(fooBar.e == (T)216.0f);
        assert(fooBar.f == (T)231.0f);
        assert(fooBar.g == (T)318.0f);
        assert(fooBar.h == (T)342.0f);
        assert(fooBar.i == (T)366.0f);

        fooBar = foo;
        fooBar *= bar;
        assert(fooBar.a == (T)84.0f);
        assert(fooBar.b == (T)90.0f);
        assert(fooBar.c == (T)96.0f);
        assert(fooBar.d == (T)201.0f);
        assert(fooBar.e == (T)216.0f);
        assert(fooBar.f == (T)231.0f);
        assert(fooBar.g == (T)318.0f);
        assert(fooBar.h == (T)342.0f);
        assert(fooBar.i == (T)366.0f);

        vec3<T> fooVec = foo * vec;
        assert(fooVec.x == (T)14.0f);
        assert(fooVec.y == (T)32.0f);
        assert(fooVec.z == (T)50.0f);

        fooBar = foo * delta;
        assert(fooBar.a == (T)2.0f);
        assert(fooBar.b == (T)4.0f);
        assert(fooBar.c == (T)6.0f);
        assert(fooBar.d == (T)8.0f);
        assert(fooBar.e == (T)10.0f);
        assert(fooBar.f == (T)12.0f);
        assert(fooBar.g == (T)14.0f);
        assert(fooBar.h == (T)16.0f);
        assert(fooBar.i == (T)18.0f);
        fooBar = foo;
        fooBar *= delta;
        assert(fooBar.a == (T)2.0f);
        assert(fooBar.b == (T)4.0f);
        assert(fooBar.c == (T)6.0f);
        assert(fooBar.d == (T)8.0f);
        assert(fooBar.e == (T)10.0f);
        assert(fooBar.f == (T)12.0f);
        assert(fooBar.g == (T)14.0f);
        assert(fooBar.h == (T)16.0f);
        assert(fooBar.i == (T)18.0f);

        return true;
    }
public:
    void run(bool _extended) override{
        extended = _extended;
        TestInitialization();
        TestUtils();
        TestMath();
    }
};

bool TestMat3(){
    Mat3TestCase<int>().run(false);
    Mat3TestCase<float>().run(true);
    Mat3TestCase<double>().run(true);
    return true;
}