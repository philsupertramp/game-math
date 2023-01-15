/**
 * @file
 *
 * Utility functions for 2,3,4-D vectors/matrices.
 *
 * This header file holds all sorts of helper functions used in computer-graphics
 * to manipulate 2,3,4-D objects.
 */
#pragma once
#include "mat/mat2.h"
#include "mat/mat3.h"
#include "mat/mat4.h"
#include "vec/vec2.h"
#include "vec/vec3.h"
#include "vec/vec4.h"

namespace Math::Utils {

    /**
     * Calculates distance between given vectors
     * @tparam V
     * @param a
     * @param b
     * @return
     */
    template<class V>
    float distance(const V& a, const V& b) {
        return (a - b).length();
    }

    /**
     * helper to normalize 2D-vector
     * @tparam T
     * @param a
     * @return
     */
    template<class T>
    vec2<T> normalize(const vec2<T>& a) {
        return a.normalize();
    }

    /**
     * helper to normalize 3D-vector
     * @tparam T
     * @param a
     * @return
     */
    template<class T>
    vec3<T> normalize(const vec3<T>& a) {
        return a.normalize();
    }

    /**
     * helper to normalize 4D-vector
     * @tparam T
     * @param a
     * @return
     */
    template<class T>
    vec4<T> normalize(const vec4<T>& a) {
        return a.normalize();
    }

    /**
     * Translates a given 4-by-4 matrix M by given vector V
     * @tparam T
     * @param M
     * @param V
     * @return
     */
    template<class T>
    mat4<T> translate(const mat4<T>& M, const vec3<T>& V) {
        mat4<T> translation = M;
        vec3<T> res         = vec3<T>(M[0][0], M[0][1], M[0][2]) * V.x + vec3<T>(M[1][0], M[1][1], M[1][2]) * V.y
                      + vec3<T>(M[2][0], M[2][1], M[2][2]) * V.z + vec3<T>(M[3][0], M[3][1], M[3][2]);
        translation[3][0] = res.x;
        translation[3][1] = res.y;
        translation[3][2] = res.z;
        return translation;
    }

    /**
     * Computes look at matrix for given data
     * @tparam T
     * @param eye
     * @param center
     * @param up
     * @return
     */
    template<class T>
    mat4<T> lookAt(const vec3<T>& eye, const vec3<T>& center, const vec3<T>& up) {
        vec3<T> const f(normalize(center - eye));
        vec3<T> const s(normalize(f.cross(up)));
        vec3<T> const u(s.cross(f));

        mat4<T> Result(1);
        Result[0][0] = s.x;
        Result[1][0] = s.y;
        Result[2][0] = s.z;
        Result[0][1] = u.x;
        Result[1][1] = u.y;
        Result[2][1] = u.z;
        Result[0][2] = -f.x;
        Result[1][2] = -f.y;
        Result[2][2] = -f.z;
        Result[3][0] = -(s * eye);
        Result[3][1] = -(u * eye);
        Result[3][2] = (f * eye);
        return Result;
    }

    /**
     * calculates orthogonal projection plane
     * @tparam T
     * @param left
     * @param right
     * @param bottom
     * @param top
     * @return
     */
    template<class T>
    mat4<T> ortho(const float& left, const float& right, const float& bottom, const float& top) {
        mat4<T> mat(1.0f);
        mat[0][0] = static_cast<T>(2) / (right - left);
        mat[1][1] = static_cast<T>(2) / (top - bottom);
        mat[2][2] = -static_cast<T>(1);
        mat[3][0] = -(right + left) / (right - left);
        mat[3][1] = -(top + bottom) / (top - bottom);
        return mat;
    }

    /**
     * Computes perspective projection matrix
     * @tparam T
     * @param FOV
     * @param width
     * @param height
     * @param zNear
     * @param zFar
     * @return
     */
    template<class T>
    mat4<T>
    perspective(const float& FOV, const float& width, const float& height, const float& zNear, const float& zFar) {
        float halfTanFOV = tanf(FOV / static_cast<T>(2));
        float aspect     = width / height;
        return mat4<T>(
        static_cast<T>(1) / (aspect * halfTanFOV),
        0,
        0,
        0,
        0,
        static_cast<T>(1) / (halfTanFOV),
        0,
        0,
        0,
        0,
        -(zFar + zNear) / (zFar - zNear),
        -static_cast<T>(1),
        0,
        0,
        -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear),
        0);
    }

    /**
     *
     * @tparam T
     * @param angle
     * @param axis
     * @return
     */
    template<class T>
    mat4<T> angleAxis([[maybe_unused]] const float& angle, [[maybe_unused]] const vec3<T>& axis) {
        //        return mat4<T>(axis.x, axis.y, axis.z, angle);
        return mat4<T>(0);
    }

    /**
     * scales a given matrix by a factor
     * @tparam T
     * @param mat
     * @param factor
     * @return
     */
    template<class T>
    mat4<T> scale(const mat4<T>& mat, const float& factor) {
        return (mat4<T>::Unit() * factor) * mat;
    }
    template<class T>
    mat4<T> scale(const mat4<T>& mat, const vec3<T>& factor) {
        mat4<T> out;
        out[0][0] = mat[0][0] * factor[0];
        out[0][1] = mat[0][1] * factor[0];
        out[0][2] = mat[0][2] * factor[0];
        out[0][3] = mat[0][3] * factor[0];
        out[1][0] = mat[1][0] * factor[1];
        out[1][1] = mat[1][1] * factor[1];
        out[1][2] = mat[1][2] * factor[1];
        out[1][3] = mat[1][3] * factor[1];
        out[2][0] = mat[2][0] * factor[2];
        out[2][1] = mat[2][1] * factor[2];
        out[2][2] = mat[2][2] * factor[2];
        out[2][3] = mat[2][3] * factor[2];
        out[3][0] = mat[3][0] * static_cast<T>(1.0);
        out[3][1] = mat[3][1] * static_cast<T>(1.0);
        out[3][2] = mat[3][2] * static_cast<T>(1.0);
        out[3][3] = mat[3][3] * static_cast<T>(1.0);
        return out;
    }

    /**
     * Inverts projection
     * @tparam T
     * @tparam U
     * @param win 3D point to convert from window space
     * @param model (model * view)-matrix
     * @param proj projection matrix
     * @param viewport 4D vector for window viewport (most likely { 0, 0, SCREEN_WIDTH, SCREEN_WIDTH })
     * @return
     */
    template<typename T, typename U>
    vec3<T> unProject(vec3<T> const& win, mat4<T> const& model, mat4<T> const& proj, vec4<U> const& viewport) {
        mat4<T> Inverse = (proj * model).Inverse();

        vec4<T> tmp(win, T(1));
        tmp.x = (tmp.x - T(viewport[0])) / T(viewport[2]);
        tmp.y = (tmp.y - T(viewport[1])) / T(viewport[3]);
        tmp.x = tmp.x * static_cast<T>(2) - static_cast<T>(1);
        tmp.y = tmp.y * static_cast<T>(2) - static_cast<T>(1);

        vec4<T> obj = Inverse * tmp;
        obj /= obj.w;

        return vec3<T>(obj);
    }

    /**
     * Rotates a given matrix m by an angle angle around given axis u
     * @tparam T
     * @param m
     * @param angle
     * @param u
     * @return
     */
    template<class T>
    mat4<T> rotate(const mat4<T>& m, const float& angle, vec3<T> u) {
        T const cosAngle = cosf(angle);
        T const sinAngle = sinf(angle);

        vec3<T> axis = u.normalize();
        vec3<T> temp((T(1) - cosAngle) * axis);

        mat4<T> rot;
        rot[0][0] = cosAngle + temp[0] * axis[0];
        rot[0][1] = temp[0] * axis[1] + sinAngle * axis[2];
        rot[0][2] = temp[0] * axis[2] - sinAngle * axis[1];

        rot[1][0] = temp[1] * axis[0] - sinAngle * axis[2];
        rot[1][1] = cosAngle + temp[1] * axis[1];
        rot[1][2] = temp[1] * axis[2] + sinAngle * axis[0];

        rot[2][0] = temp[2] * axis[0] + sinAngle * axis[1];
        rot[2][1] = temp[2] * axis[1] - sinAngle * axis[0];
        rot[2][2] = cosAngle + temp[2] * axis[2];

        const vec4<T> out0 = vec4(m[0]) * rot[0][0] + vec4(m[1]) * rot[0][1] + vec4(m[2]) * rot[0][2];
        const vec4<T> out1 = vec4(m[0]) * rot[1][0] + vec4(m[1]) * rot[1][1] + vec4(m[2]) * rot[1][2];
        const vec4<T> out2 = vec4(m[0]) * rot[2][0] + vec4(m[1]) * rot[2][1] + vec4(m[2]) * rot[2][2];
        const vec4<T> out3 = vec4(m[3]);
        return mat4<T>(out0, out1, out2, out3);
    }

    /**
     * Computes max-value between two given 3D-vectors
     * @tparam T
     * @param a
     * @param b
     * @return
     */
    template<class T>
    vec3<T> max(const vec3<T>& a, const vec3<T>& b) {
        return a.length() > b.length() ? a : b;
    }

    /**
     * linear interpolates a 3D-vector by given values
     * @tparam T
     * @param p1
     * @param p2
     * @param v
     * @return
     */
    template<class T>
    vec3<T> lerp(const vec3<T>& p1, const vec3<T>& p2, const float& v) {
        T ax = p1[0];
        T ay = p1[1];
        T az = p1[2];
        return vec3<T>(ax + v * (p2[0] - ax), ay + v * (p2[1] - ay), az + v * (p2[2] - az));
    }

    /**
     * linear interpolates a 4D-vector by given values
     * @tparam T
     * @param p1
     * @param p2
     * @param v
     * @return
     */
    template<class T>
    vec4<T> lerp(const vec4<T>& p1, const vec4<T>& p2, const float& v) {
        T ax = p1[0];
        T ay = p1[1];
        T az = p1[2];
        T aw = p1[3];
        return vec4<T>(ax + v * (p2[0] - ax), ay + v * (p2[1] - ay), az + v * (p2[2] - az), aw + v * (p2[3] - aw));
    }
} // namespace Math::Utils

/**
 * cast from vec2 to void*
 * @tparam T
 * @param vec
 * @return
 */
template<class T>
void* value_ptr(vec2<T>& vec) {
    return &(vec.x);
}

/**
 * cast from vec3 to void*
 * @tparam T
 * @param vec
 * @return
 */
template<class T>
void* value_ptr(vec3<T>& vec) {
    return &(vec.x);
}

/**
 * cast from vec4 to void*
 * @tparam T
 * @param vec
 * @return
 */
template<class T>
void* value_ptr(vec4<T>& vec) {
    return &(vec.x);
}
/**
 * cast from mat2 to void*
 * @tparam T
 * @param mat
 * @return
 */
template<class T>
void* value_ptr(mat2<T>& mat) {
    return &(mat[0][0]);
}
/**
 * cast from mat3 to void*
 * @tparam T
 * @param mat
 * @return
 */
template<class T>
void* value_ptr(mat3<T>& mat) {
    return &(mat[0][0]);
}

/**
 * cast from mat4 to void*
 * @tparam T
 * @param mat
 * @return
 */
template<class T>
void* value_ptr(mat4<T>& mat) {
    return &(mat[0][0]);
}

/**
 * cast from const vec2 to void*
 * @tparam T
 * @param vec
 * @return
 */
template<class T>
const void* value_ptr(vec2<T> const& vec) {
    return &(vec.x);
}

/**
 * cast from const vec3 to void*
 * @tparam T
 * @param vec
 * @return
 */
template<class T>
const void* value_ptr(vec3<T> const& vec) {
    return &(vec.x);
}
/**
 * cast from const vec4 to void*
 * @tparam T
 * @param vec
 * @return
 */
template<class T>
const void* value_ptr(vec4<T> const& vec) {
    return &(vec.x);
}
/**
 * cast from const mat2 to void*
 * @tparam T
 * @param mat
 * @return
 */
template<class T>
const void* value_ptr(mat2<T> const& mat) {
    return &(mat[0][0]);
}
/**
 * cast from const mat3 to void*
 * @tparam T
 * @param mat
 * @return
 */
template<class T>
const void* value_ptr(mat3<T> const& mat) {
    return &(mat[0][0]);
}
/**
 * cast from mat3 to void*
 * @tparam T
 * @param mat
 * @return
 */
template<class T>
const void* value_ptr(mat4<T> const& mat) {
    return &(mat[0][0]);
}

/**
 * Calculates sign(d)
 *
 * @param d value to test
 * @return -1 if d < 0 else 1
 */
template<class T>
T sign(const T& d) {
    if(d == 0.f) return 1;
    return d < 0 ? -1 : 1;
}

/**
 * Absolute value of given value.
 * $f(x) = |x|$
 *
 * @tparam T datatype of input value
 * @param d input value
 * @return |d|
 */
template<class T>
T abs(const T& d) {
    if(d < 0.f) return static_cast<T>(-1) * d;
    return d;
}


/**
 * \example TestUtils.cpp
 * This is an example on how to use the utils.h file.
 */
