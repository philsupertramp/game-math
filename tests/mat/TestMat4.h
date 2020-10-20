#pragma once

#include <cassert>
#include <iostream>
#include <math/math.h>  // NOLINT
#include "../Test.h"

template<class T>
class Mat4TestCase
: public Test
{
    bool TestInitialization() {
        mat4<T> foo;
        assert(foo.a == (T) 0);
        assert(foo.b == (T) 0);
        assert(foo.c == (T) 0);
        assert(foo.d == (T) 0);
        assert(foo.e == (T) 0);
        assert(foo.f == (T) 0);
        assert(foo.g == (T) 0);
        assert(foo.h == (T) 0);
        assert(foo.i == (T) 0);
        assert(foo.j == (T) 0);
        assert(foo.k == (T) 0);
        assert(foo.l == (T) 0);
        assert(foo.m == (T) 0);
        assert(foo.n == (T) 0);
        assert(foo.o == (T) 0);
        assert(foo.p == (T) 0);

        return true;
    }
    bool TestUtils(){
        mat4<T> foo = mat4<T>::Unit();
        assert(foo.a == (T) 1);
        assert(foo.b == (T) 0);
        assert(foo.c == (T) 0);
        assert(foo.d == (T) 0);
        assert(foo.e == (T) 0);
        assert(foo.f == (T) 1);
        assert(foo.g == (T) 0);
        assert(foo.h == (T) 0);
        assert(foo.i == (T) 0);
        assert(foo.j == (T) 0);
        assert(foo.k == (T) 1);
        assert(foo.l == (T) 0);
        assert(foo.m == (T) 0);
        assert(foo.n == (T) 0);
        assert(foo.o == (T) 0);
        assert(foo.p == (T) 1);

        foo = mat4<T>(
                (T) 1, (T) 2, (T) 2, (T) 2,
                (T) 2, (T) 1, (T) 2, (T) 2,
                (T) 2, (T) 2, (T) 1, (T) 2,
                (T) 2, (T) 2, (T) 2, (T)1);
        assert(foo.a == (T) 1);
        assert(foo.b == (T) 2);
        assert(foo.c == (T) 2);
        assert(foo.d == (T) 2);
        assert(foo.e == (T) 2);
        assert(foo.f == (T) 1);
        assert(foo.g == (T) 2);
        assert(foo.h == (T) 2);
        assert(foo.i == (T) 2);
        assert(foo.j == (T) 2);
        assert(foo.k == (T) 1);
        assert(foo.l == (T) 2);
        assert(foo.m == (T) 2);
        assert(foo.n == (T) 2);
        assert(foo.o == (T) 2);
        assert(foo.p == (T) 1);

        assert(foo.IsSymmetric());

        mat4<T> fooCopy = foo;
        assert(fooCopy.a == foo.a);
        assert(fooCopy.b == foo.b);
        assert(fooCopy.c == foo.c);
        assert(fooCopy.d == foo.d);
        assert(fooCopy.e == foo.e);
        assert(fooCopy.f == foo.f);
        assert(fooCopy.g == foo.g);
        assert(fooCopy.h == foo.h);
        assert(fooCopy.i == foo.i);
        assert(fooCopy.j == foo.j);
        assert(fooCopy.k == foo.k);
        assert(fooCopy.l == foo.l);
        assert(fooCopy.m == foo.m);
        assert(fooCopy.n == foo.n);
        assert(fooCopy.o == foo.o);
        assert(fooCopy.p == foo.p);

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
        assert(fooCopy.j == foo.j);
        assert(fooCopy.k == foo.k);
        assert(fooCopy.l == foo.l);
        assert(fooCopy.m == foo.m);
        assert(fooCopy.n == foo.n);
        assert(fooCopy.o == foo.o);
        assert(fooCopy.p == foo.p);

        fooCopy = mat4<T>(
                (T) 0, (T) 1, (T) 1, (T) 1,
                (T) -1, (T) 0, (T) 1, (T) 1,
                (T) -1, (T) -1, (T) 0, (T) 1,
                (T) -1, (T) -1, (T) -1, (T)0);
        fooCopy = fooCopy.Transpose();
        assert(fooCopy.a == (T)  0);
        assert(fooCopy.b == (T) -1);
        assert(fooCopy.c == (T) -1);
        assert(fooCopy.d == (T) -1);
        assert(fooCopy.e == (T)  1);
        assert(fooCopy.f == (T)  0);
        assert(fooCopy.g == (T) -1);
        assert(fooCopy.h == (T) -1);
        assert(fooCopy.i == (T) 1);
        assert(fooCopy.j == (T) 1);
        assert(fooCopy.k == (T) 0);
        assert(fooCopy.l == (T) -1);
        assert(fooCopy.m == (T) 1);
        assert(fooCopy.n == (T) 1);
        assert(fooCopy.o == (T) 1);
        assert(fooCopy.p == (T) 0);

        std::cout << "mat4\n" << foo;

        return true;
    }

    bool TestMath(){
        mat4<T> foo(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
        mat4<T> bar(17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32);
        vec4<T> vec(1,2,3,4);
        float delta = 2.0f;
        mat4<T> fooBar;

        // +
        fooBar = foo + bar;
        assert(fooBar.a == (T)18.0f);
        assert(fooBar.b == (T)20.0f);
        assert(fooBar.c == (T)22.0f);
        assert(fooBar.d == (T)24.0f);
        assert(fooBar.e == (T)26.0f);
        assert(fooBar.f == (T)28.0f);
        assert(fooBar.g == (T)30.0f);
        assert(fooBar.h == (T)32.0f);
        assert(fooBar.i == (T)34.0f);
        assert(fooBar.j == (T)36.0f);
        assert(fooBar.k == (T)38.0f);
        assert(fooBar.l == (T)40.0f);
        assert(fooBar.m == (T)42.0f);
        assert(fooBar.n == (T)44.0f);
        assert(fooBar.o == (T)46.0f);
        assert(fooBar.p == (T)48.0f);

        fooBar = foo;
        fooBar += bar;
        assert(fooBar.a == (T)18.0f);
        assert(fooBar.b == (T)20.0f);
        assert(fooBar.c == (T)22.0f);
        assert(fooBar.d == (T)24.0f);
        assert(fooBar.e == (T)26.0f);
        assert(fooBar.f == (T)28.0f);
        assert(fooBar.g == (T)30.0f);
        assert(fooBar.h == (T)32.0f);
        assert(fooBar.i == (T)34.0f);
        assert(fooBar.j == (T)36.0f);
        assert(fooBar.k == (T)38.0f);
        assert(fooBar.l == (T)40.0f);
        assert(fooBar.m == (T)42.0f);
        assert(fooBar.n == (T)44.0f);
        assert(fooBar.o == (T)46.0f);
        assert(fooBar.p == (T)48.0f);

        // -
        fooBar = foo - bar;
        assert(fooBar.a == (T)-16.0f);
        assert(fooBar.b == (T)-16.0f);
        assert(fooBar.c == (T)-16.0f);
        assert(fooBar.d == (T)-16.0f);
        assert(fooBar.e == (T)-16.0f);
        assert(fooBar.f == (T)-16.0f);
        assert(fooBar.g == (T)-16.0f);
        assert(fooBar.h == (T)-16.0f);
        assert(fooBar.i == (T)-16.0f);
        assert(fooBar.j == (T)-16.0f);
        assert(fooBar.k == (T)-16.0f);
        assert(fooBar.l == (T)-16.0f);
        assert(fooBar.m == (T)-16.0f);
        assert(fooBar.n == (T)-16.0f);
        assert(fooBar.o == (T)-16.0f);
        assert(fooBar.p == (T)-16.0f);

        fooBar = foo;
        fooBar -= bar;
        assert(fooBar.a == (T)-16.0f);
        assert(fooBar.b == (T)-16.0f);
        assert(fooBar.c == (T)-16.0f);
        assert(fooBar.d == (T)-16.0f);
        assert(fooBar.e == (T)-16.0f);
        assert(fooBar.f == (T)-16.0f);
        assert(fooBar.g == (T)-16.0f);
        assert(fooBar.h == (T)-16.0f);
        assert(fooBar.i == (T)-16.0f);
        assert(fooBar.j == (T)-16.0f);
        assert(fooBar.k == (T)-16.0f);
        assert(fooBar.l == (T)-16.0f);
        assert(fooBar.m == (T)-16.0f);
        assert(fooBar.n == (T)-16.0f);
        assert(fooBar.o == (T)-16.0f);
        assert(fooBar.p == (T)-16.0f);


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
        assert(fooBar.j == (T)5.0f);
        assert(fooBar.k == (T)5.5f);
        assert(fooBar.l == (T)6.0f);
        assert(fooBar.m == (T)6.5f);
        assert(fooBar.n == (T)7.0f);
        assert(fooBar.o == (T)7.5f);
        assert(fooBar.p == (T)8.0f);
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
        assert(fooBar.j == (T)5.0f);
        assert(fooBar.k == (T)5.5f);
        assert(fooBar.l == (T)6.0f);
        assert(fooBar.m == (T)6.5f);
        assert(fooBar.n == (T)7.0f);
        assert(fooBar.o == (T)7.5f);
        assert(fooBar.p == (T)8.0f);

        // *
        fooBar = foo * bar;
        assert(fooBar.a == (T)250.0f);
        assert(fooBar.b == (T)260.0f);
        assert(fooBar.c == (T)270.0f);
        assert(fooBar.d == (T)280.0f);
        assert(fooBar.e == (T)618.0f);
        assert(fooBar.f == (T)644.0f);
        assert(fooBar.g == (T)670.0f);
        assert(fooBar.h == (T)696.0f);
        assert(fooBar.i == (T)986.0f);
        assert(fooBar.j == (T)1028.0f);
        assert(fooBar.k == (T)1070.0f);
        assert(fooBar.l == (T)1112.0f);
        assert(fooBar.m == (T)1354.0f);
        assert(fooBar.n == (T)1412.0f);
        assert(fooBar.o == (T)1470.0f);
        assert(fooBar.p == (T)1528.0f);

        fooBar = foo;
        fooBar *= bar;
        assert(fooBar.a == (T)250.0f);
        assert(fooBar.b == (T)260.0f);
        assert(fooBar.c == (T)270.0f);
        assert(fooBar.d == (T)280.0f);
        assert(fooBar.e == (T)618.0f);
        assert(fooBar.f == (T)644.0f);
        assert(fooBar.g == (T)670.0f);
        assert(fooBar.h == (T)696.0f);
        assert(fooBar.i == (T)986.0f);
        assert(fooBar.j == (T)1028.0f);
        assert(fooBar.k == (T)1070.0f);
        assert(fooBar.l == (T)1112.0f);
        assert(fooBar.m == (T)1354.0f);
        assert(fooBar.n == (T)1412.0f);
        assert(fooBar.o == (T)1470.0f);
        assert(fooBar.p == (T)1528.0f);

        vec4<T> fooVec = foo * vec;
        assert(fooVec.x == (T)30.0f);
        assert(fooVec.y == (T)70.0f);
        assert(fooVec.z == (T)110.0f);
        assert(fooVec.w == (T)150.0f);

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
        assert(fooBar.j == (T)20.0f);
        assert(fooBar.k == (T)22.0f);
        assert(fooBar.l == (T)24.0f);
        assert(fooBar.m == (T)26.0f);
        assert(fooBar.n == (T)28.0f);
        assert(fooBar.o == (T)30.0f);
        assert(fooBar.p == (T)32.0f);
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
        assert(fooBar.j == (T)20.0f);
        assert(fooBar.k == (T)22.0f);
        assert(fooBar.l == (T)24.0f);
        assert(fooBar.m == (T)26.0f);
        assert(fooBar.n == (T)28.0f);
        assert(fooBar.o == (T)30.0f);
        assert(fooBar.p == (T)32.0f);

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

bool TestMat4(){
    Mat4TestCase<int>().run(false);
    Mat4TestCase<float>().run(true);
    Mat4TestCase<double>().run(true);
    return true;
}