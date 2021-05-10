#pragma once

#include "../vec/vec2.h"

/**
 * 2D-Matrix representation
 * @tparam T
 */
template<class T>
struct mat2 {
public:
    //! loosely packed data
    T values[2][2];

    /**
     * default constructor, initializes null matrix
     */
    mat2() {
        // clang-format off
        values[0][0] = static_cast<T>(0); values[0][1] = static_cast<T>(0);
        values[1][0] = static_cast<T>(0); values[1][1] = static_cast<T>(0); // clang-format on
    }

    /**
     * mat2(a, b, c, d) -> [a, b, c, d]
     * @param _a m_11
     * @param _b m_12
     * @param _c m_21
     * @param _d m_22
     */
    mat2(T _a, T _b, T _c, T _d) {
        // clang-format off
        values[0][0] = _a; values[0][1] = _b;
        values[1][0] = _c; values[1][1] = _d; // clang-format on
    }

    /**
     * mat2(A, B) -> [A.x, B.x, A.y, B.y]
     * @param A first column vector
     * @param B second column vector
     */
    mat2(vec2<T> A, vec2<T> B) {
        // clang-format off
        values[0][0] = A.x; values[0][1] = B.x;
        values[1][0] = A.y; values[1][1] = B.y; // clang-format on
    }

    /**
     * default destructor
     */
    ~mat2() = default;

    /**
     * generator for unit matrix
     * @return
     */
    static inline mat2<T> Unit() {
        return mat2<T>(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    /**
     * computes transposed
     * @return
     */
    inline mat2<T> Transpose() { return mat2<T>(values[0][0], values[1][0], values[0][1], values[1][1]); }

    /**
     * test if matrix is symmetric
     * @return
     */
    inline bool IsSymmetric() { return values[1][0] == values[0][1]; }
    /**
     * getter for matrix determinant
     * @return
     */
    inline float Determinant() { return values[0][0] * values[1][1] - values[0][1] * values[1][0]; }

    /**
     * matrix-matrix addition
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat2<T> operator+(mat2<T> lhs, const mat2<T>& rhs) { return lhs += rhs; }

    /**
     * matrix-matrix subtraction
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat2<T> operator-(mat2<T> lhs, const mat2<T>& rhs) { return lhs -= rhs; }

    /**
     * matrix-scalar multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat2<T> operator*(mat2<T> lhs, const T& rhs) { return lhs *= rhs; }

    /**
     * matrix-matrix multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec2<T> operator*(mat2<T> lhs, const vec2<T>& rhs) {
        return vec2<T>(lhs[0][0] * rhs.x + lhs[0][1] * rhs.y, lhs[1][0] * rhs.x + lhs[1][1] * rhs.y);
    }

    /**
     * matrix-matrix multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat2<T> operator*(mat2<T> lhs, const mat2<T>& rhs) { return lhs *= rhs; }

    /**
     * matrix-scalar division
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat2<T> operator/(mat2<T> lhs, const T& rhs) { return lhs /= rhs; }

    /* compound assignment */
    /**
     * matrix-matrix addition
     * @param rhs
     * @return
     */
    mat2<T>& operator+=(const mat2<T>& rhs) {
        // clang-format off
        values[0][0] += rhs[0][0]; values[0][1] += rhs[0][1];
        values[1][0] += rhs[1][0]; values[1][1] += rhs[1][1]; // clang-format on
        return *this;
    }

    /**
     * matrix-matrix subtraction
     * @param rhs
     * @return
     */
    mat2<T>& operator-=(const mat2<T>& rhs) {
        // clang-format off
        values[0][0] -= rhs[0][0]; values[0][1] -= rhs[0][1];
        values[1][0] -= rhs[1][0]; values[1][1] -= rhs[1][1]; // clang-format on
        return *this;
    }

    /**
     * matrix-matrix multiplication
     * @param rhs
     * @return
     */
    mat2<T>& operator*=(const mat2<T>& rhs) {
        // clang-format off
        T _a = values[0][0], _b = values[0][1],
          _c = values[1][0], _d = values[1][1];
        values[0][0] = _a * rhs[0][0] + _b * rhs[1][0]; values[0][1] = _a * rhs[0][1] + _b * rhs[1][1];
        values[1][0] = _c * rhs[0][0] + _d * rhs[1][0]; values[1][1] = _c * rhs[0][1] + _d * rhs[1][1]; // clang-format on
        return *this;
    }

    /**
     * matrix-scalar multiplication
     * @param rhs
     * @return
     */
    mat2<T>& operator*=(const T& rhs) {
        // clang-format off
        values[0][0] *= rhs; values[0][1] *= rhs;
        values[1][0] *= rhs; values[1][1] *= rhs; // clang-format on
        return *this;
    }

    /**
     * matrix-scalar division
     * @param rhs
     * @return
     */
    mat2<T>& operator/=(const T& rhs) {
        // clang-format off
        values[0][0] /= rhs; values[0][1] /= rhs;
        values[1][0] /= rhs; values[1][1] /= rhs; // clang-format on
        return *this;
    }

    /* Member access */
    /**
     * element access
     * @param index
     * @return
     */
    T* operator[](int index) { return values[index]; }
    /**
     * element access
     * @param index
     * @return
     */
    const T* operator[](int index) const { return values[index]; }

    /* stream operators */
    /**
     * beautified std::cout operator
     * @tparam U
     * @return
     */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const mat2<U>&);
};

template<class U>
std::ostream& operator<<(std::ostream& out, const mat2<U>& mat) {
    out.precision(17);
    out << "[\n\t" << mat[0][0] << ", " << mat[0][1] << ";\n\t" << mat[1][0] << ", " << mat[1][1] << "\n]\n";
    return out;
}
