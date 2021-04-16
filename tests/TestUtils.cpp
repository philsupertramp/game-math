#include "Test.h"
#include <math/Matrix.h>
#include <math/math.h>


class UtilsTestCase : public Test
{
    bool TestDistance() {
        vec3f a(0, 0, 0);
        vec3f b(2, 0, 0);

        AssertEqual(Math::Utils::distance(a, b), 2.0f);
        b = { -2, 0, 0 };
        AssertEqual(Math::Utils::distance(a, b), 2.0f);

        return true;
    }
    bool TestNormalize() {
        vec2f a2(3, 6);
        vec2f b2 = a2.normalize();
        AssertEqual(Math::Utils::normalize(a2), b2);

        vec3f a3(0, 3, 6);
        vec3f b3 = a3.normalize();
        AssertEqual(Math::Utils::normalize(a3), b3);

        vec4f a4(0, 3, 6, 9);
        vec4f b4 = a4.normalize();
        AssertEqual(Math::Utils::normalize(a4), b4);
        return true;
    }
    bool TestTranslate() {
        mat4f model(0.0f);
        auto b = Math::Utils::translate(model, { 1.0, 1.0, 1.0 });

        AssertEqual(model, b);
        return true;
    }
    bool TestLookAt() {
        vec3f eye(1, 0, 1);
        vec3f center(0, 0, 0);
        vec3f up(0, 1, 0);

        auto res = Math::Utils::lookAt(eye, center, up);
        mat4f expected(
        { 0.7071068286895752, 0, 0.70710676908493042, 0 },
        { 0, 1, -0, 0 },
        { -0.7071068286895752, 0, 0.70710676908493042, 0 },
        { -0, -0, -1.4142135381698608, 1 });

        AssertEqual(res, expected);
        return true;
    }
    bool TestOrtho() {
        auto res = Math::Utils::ortho<float>(0, 16, 0, 9);

        mat4f expected = { { 0.125, 0, 0, 0 }, { 0, 0.222222222222222, 0, 0 }, { 0, 0, -1, 0 }, { -1, -1, 0, 1 } };
        AssertEqual(res, expected);
        return true;
    }
    bool TestPerspective() {
        auto res = Math::Utils::perspective<float>(35, 800, 600, 0.1, 100000.0);

        mat4f expected = { { -0.16869165003299713, 0, 0, 0 },
                           { 0, -0.22492220997810364, 0, 0 },
                           { 0, 0, -1.0000020265579224, -1 },
                           { 0, 0, -0.20000019669532776, 0 } };
        AssertEqual(res, expected);
        return true;
    }
    bool TestAngleAxis() {
        vec3f a(1, 1, 1);
        auto res = Math::Utils::angleAxis(0, a);

        mat4f expected(0);
        AssertEqual(res, expected);
        return true;
    }
    bool TestScale() {
        mat4f model(1.0f);
        auto res = Math::Utils::scale(model, 1);

        AssertEqual(model, res);
        res = Math::Utils::scale(model, 2);

        auto expected = model * 2.0f;

        AssertEqual(res, expected);

        return true;
    }
    bool TestRotate() {
        mat4f a(1.0f);
        vec3f b(1.0f, 0, 0);
        auto res = Math::Utils::rotate(a, 90, b);

        mat4f expected({ { 1, 0, 0, 0 },
                         { 0, -0.4480736255645752, 0.89399665594100952, 0 },
                         { 0, -0.89399665594100952, -0.4480736255645752, 0 },
                         { 0, 0, 0, 1 } });
        AssertEqual(res, expected);
        return true;
    }
    bool TestMax() {
        vec3f a(1, 0, 0);
        vec3f b(2, 0, 0);

        AssertEqual(Math::Utils::max(a, b), b);
        return true;
    }
    bool TestLerp() {
        vec3f a(1, 0, 0);
        vec3f b(2, 0, 0);
        auto res = Math::Utils::lerp(a, b, 0.0);

        AssertEqual(res, a);

        res = Math::Utils::lerp(b, a, 0.0);

        AssertEqual(res, b);

        res = Math::Utils::lerp(a, b, 0.5);

        AssertEqual(res, { 1.5, 0, 0 });

        vec4f a4(1, 0, 0, 0);
        vec4f b4(2, 0, 0, 0);
        auto res4 = Math::Utils::lerp(a4, b4, 0.0);

        AssertEqual(res4, a4);
        return true;
    }
    bool TestValuePtr() {
        vec2f a(1, 2);
        vec3f b(1, 2, 3);
        vec4f c(1, 2, 3, 4);

        mat2f m({ 1, 2 }, { 3, 4 });
        mat3f n({ 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 });
        mat4f p({ 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 });

        auto* a2 = (float*)value_ptr(a);
        for(int i = 0; i < 2; ++i) { AssertEqual(a2[i], a[i]); }
        auto* b2 = (float*)value_ptr(b);
        for(int i = 0; i < 3; ++i) { AssertEqual(b2[i], b[i]); }
        auto* c2 = (float*)value_ptr(c);
        for(int i = 0; i < 4; ++i) { AssertEqual(c2[i], c[i]); }

        auto* m2 = (float*)value_ptr(m);

        for(int i = 0; i < 2; ++i) {
            for(int j = 0; j < 2; ++j) { AssertEqual(m2[i * 2 + j], m[i][j]); }
        }
        auto* n2 = (float*)value_ptr(n);
        for(int i = 0; i < 3; ++i) {
            for(int j = 0; j < 3; ++j) { AssertEqual(n2[i * 3 + j], n[i][j]); }
        }
        auto* p2 = (float*)value_ptr(p);

        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) { AssertEqual(p2[i * 4 + j], p[i][j]); }
        }
        return true;
    }

public:
    void run() override {
        TestDistance();
        TestNormalize();
        TestTranslate();
        TestLookAt();
        TestLerp();
        TestOrtho();
        TestPerspective();
        TestAngleAxis();
        TestScale();
        TestRotate();
        TestMax();
        TestValuePtr();
    }
};

int main() {
    UtilsTestCase().run();
    return 0;
}
