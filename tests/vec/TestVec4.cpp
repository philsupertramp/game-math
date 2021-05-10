#include "../Test.h"
#include <math/math.h> // NOLINT
#include <sstream>


template<class T>
class Vec4TestCase : public Test
{
    bool testInitialization() {
        vec4<T> foo;
        assert(foo.x == (T)0);
        assert(foo.y == (T)0);
        assert(foo.z == (T)0);
        assert(foo.w == (T)0);

        vec4<T> foo2((T)1, (T)2, (T)3, (T)4);
        assert(foo2.x == (T)1);
        assert(foo2.y == (T)2);
        assert(foo2.z == (T)3);
        assert(foo2.w == (T)4);

        T vals[4]   = { 2, 4, 6, 8 };
        vec4<T> vec = build_vec4<T>((void*)vals);
        assert(vec.x == (T)2);
        assert(vec.y == (T)4);
        assert(vec.z == (T)6);
        assert(vec.w == (T)8);

        vec4<T> vec2(vals);
        assert(vec2.x == (T)2);
        assert(vec2.y == (T)4);
        assert(vec2.z == (T)6);
        assert(vec2.w == (T)8);

        return true;
    }
    bool testAssignment() {
        vec4<T> foo;

        foo.x = (T)1;
        foo.y = (T)2;
        foo.z = (T)3;
        foo.w = (T)4;

        assert(foo.x == (T)1);
        assert(foo.y == (T)2);
        assert(foo.z == (T)3);
        assert(foo.w == (T)4);

        foo[0] = (T)5;
        foo[1] = (T)6;
        foo[2] = (T)7;
        foo[3] = (T)8;

        assert(foo.x == (T)5);
        assert(foo.y == (T)6);
        assert(foo.z == (T)7);
        assert(foo.w == (T)8);
        return true;
    }
    bool testMemberAccess() {
        vec4<T> foo;

        foo.x = (T)1;
        foo.y = (T)2;
        foo.z = (T)3;
        foo.w = (T)4;

        assert(foo[0] == 1);
        assert(foo[1] == 2);
        assert(foo[2] == 3);
        assert(foo[3] == 4);
        return true;
    }
    bool testMisc() {
        vec4<T> foo(2, 2, 2, 2);
        vec4<T> bar(2, -2, 2, 2);
        vec4<T> baz(10, 0, 2, 2);

        assert(foo.length() == 4.0f);
        assert(bar.length() == 4.0f);
        assert(baz.length() == 10.3923044f);

        if(extended) {
            vec4<T> normFoo = foo.normalize();
            vec4<T> normBar = bar.normalize();
            vec4<T> normBaz = baz.normalize();

            // floating point precision 1e-7
            assert(normFoo.x - (T)0.5f < 1e-7);
            assert(normFoo.y - (T)0.5f < 1e-7);
            assert(normFoo.z - (T)0.5f < 1e-7);
            assert(normFoo.w - (T)0.5f < 1e-7);
            assert(normBar.x - (T)0.5f < 1e-7);
            assert(normBar.y - (T)-0.5f < 1e-7);
            assert(normBar.z - (T)0.5f < 1e-7);
            assert(normBar.w - (T)0.5f < 1e-7);
            assert(normBaz.x - (T)0.962250471f < 1e-7);
            assert(normBaz.y - (T)0.0f < 1e-7);
            assert(normBaz.z - (T)0.192450091f < 1e-7);
            assert(normBaz.w - (T)0.192450091f < 1e-7);
        }
        return true;
    }
    bool testOperators() {
        vec4<T> foo(1, 1, 1, 1);
        vec4<T> bar(1, -1, 1, 1);
        vec4<T> baz(10, 0, 0, 0);
        // +
        vec4<T> fooCopy = foo;
        fooCopy += bar;
        assert(fooCopy.x == 2.0f);
        assert(fooCopy.y == 0.0f);
        assert(fooCopy.z == 2.0f);
        assert(fooCopy.w == 2.0f);
        fooCopy = foo + bar;
        assert(fooCopy.x == 2.0f);
        assert(fooCopy.y == 0.0f);
        assert(fooCopy.z == 2.0f);
        assert(fooCopy.w == 2.0f);
        fooCopy = foo;

        // -
        fooCopy -= bar;
        assert(fooCopy.x == 0.0f);
        assert(fooCopy.y == 2.0f);
        assert(fooCopy.z == 0.0f);
        assert(fooCopy.w == 0.0f);
        fooCopy = foo - bar;
        assert(fooCopy.x == 0.0f);
        assert(fooCopy.y == 2.0f);
        assert(fooCopy.z == 0.0f);
        assert(fooCopy.w == 0.0f);
        fooCopy = foo;

        // *
        fooCopy *= 2.0f;
        assert(fooCopy.x == (T)2.0f);
        assert(fooCopy.y == (T)2.0f);
        assert(fooCopy.z == (T)2.0f);
        assert(fooCopy.w == (T)2.0f);
        fooCopy = foo * 2.0f;
        assert(fooCopy.x == (T)2.0f);
        assert(fooCopy.y == (T)2.0f);
        assert(fooCopy.z == (T)2.0f);
        assert(fooCopy.w == (T)2.0f);

        float fooVal = foo * bar;
        assert(fooVal == (T)2.0f);
        fooVal = foo * baz;
        assert(fooVal == (T)11.0f);
        fooCopy = foo;

        // /
        fooCopy /= 2.0f;
        assert(fooCopy.x == (T)0.5f);
        assert(fooCopy.y == (T)0.5f);
        assert(fooCopy.z == (T)0.5f);
        assert(fooCopy.w == (T)0.5f);
        fooCopy = foo / 2.0f;
        assert(fooCopy.x == (T)0.5f);
        assert(fooCopy.y == (T)0.5f);
        assert(fooCopy.z == (T)0.5f);
        assert(fooCopy.w == (T)0.5f);

        // ostream<<
        //        std::cout << "vec4:\n" << foo;

        return true;
    }

    bool testComparison() {
        vec4<T> a(1, 2, 3, 4);
        vec4<T> b(1, 2, 3, 4);

        assert(a == b);
        return true;
    }

    bool testCast() {
        vec4<T> a(1, 2, 3, 4);
        vec4<int> a2(1, 2, 3, 4);
        auto b = vec4<int>(a);

        assert(a2 == b);
        return true;
    }

public:
    void run() override {
        testInitialization();
        testAssignment();
        testMemberAccess();
        testMisc();
        testOperators();
        testCast();
        testComparison();
    }
};

int main() {
    Vec4TestCase<int>().run();
    auto testCase     = Vec4TestCase<float>();
    testCase.extended = true;
    testCase.run();
    auto testCase2     = Vec4TestCase<double>();
    testCase2.extended = true;
    testCase2.run();

    return 0;
}
