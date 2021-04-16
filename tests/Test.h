#pragma once

#include <cassert>
#include <iostream>
#include <math/math.h>
#include <math/numerics/utils.h>

#define TESTING_EPS 1e-10
class Test
{
public:
    bool extended      = false;
    virtual void run() = 0;

    void AssertEqual(const double& a, const double& b) { assert(std::abs(a - b) <= TESTING_EPS); }
    void AssertEqual(const int& a, const int& b) { assert(abs(a - b) <= TESTING_EPS); }
    template<typename T>
    void AssertEqual(const vec2<T>& a, const vec2<T>& b) {
        assert(Math::Utils::distance(a, b) <= TESTING_EPS);
    }
    template<typename T>
    void AssertEqual(const vec3<T>& a, const vec3<T>& b) {
        assert(Math::Utils::distance(a, b) <= TESTING_EPS);
    }
    template<typename T>
    void AssertEqual(const vec4<T>& a, const vec4<T>& b) {
        assert(Math::Utils::distance(a, b) <= TESTING_EPS);
    }
    template<typename T>
    void AssertEqual(const mat2<T>& a, const mat2<T>& b) {
        for(size_t i = 0; i < 2; i++) {
            for(size_t j = 0; j < 2; ++j) { AssertEqual(a[i][j], b[i][j]); }
        }
    }
    template<typename T>
    void AssertEqual(const mat3<T>& a, const mat3<T>& b) {
        for(size_t i = 0; i < 3; i++) {
            for(size_t j = 0; j < 3; ++j) { AssertEqual(a[i][j], b[i][j]); }
        }
    }
    template<typename T>
    void AssertEqual(const mat4<T>& a, const mat4<T>& b) {
        for(size_t i = 0; i < 4; i++) {
            for(size_t j = 0; j < 4; ++j) { AssertEqual(a[i][j], b[i][j]); }
        }
    }
    template<typename T>
    void AssertEqual(const Matrix<T>& a, const Matrix<T>& b) {
        a.assertSize(b);
        for(int i = 0; i < a.rows(); ++i) {
            for(int j = 0; j < a.columns(); ++j) { AssertEqual(a(i, j), b(i, j)); }
        }
    }
};
