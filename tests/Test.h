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

    template<
    typename U,
    typename T,
    typename = std::enable_if_t<(std::is_arithmetic<T>::value) && (std::is_arithmetic<U>::value)>>
    void AssertEqual(const U& a, const T& b) {
        assert(std::abs(double(a) - double(b)) <= TESTING_EPS);
    }
    void AssertEqual(const std::string& a, const std::string& b) { assert(a.compare(b) == 0); }
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
        for(size_t i = 0; i < a.rows(); ++i) {
            for(size_t j = 0; j < a.columns(); ++j) {
                for(size_t c = 0; c < a.elements(); ++c) { AssertEqual(a(i, j, c), b(i, j, c)); }
            }
        }
    }

    void AssertTrue(const bool& a) { assert(a); }
    void AssertFalse(const bool& a) { assert(!a); }
};
