#pragma once

#include "../vec/vec3.h"

/**
 * 3D-Matrix representation
 * @tparam T
 */
template<class T>
struct mat3 {
public:
    //! loosely packed data
    T values[3][3];

    /**
     * default constructor, initializes null matrix
     */
    mat3() {
        // clang-format off
        values[0][0] = static_cast<T>(0); values[0][1] = static_cast<T>(0); values[0][2] = static_cast<T>(0);
        values[1][0] = static_cast<T>(0); values[1][1] = static_cast<T>(0); values[1][2] = static_cast<T>(0);
        values[2][0] = static_cast<T>(0); values[2][1] = static_cast<T>(0); values[2][2] = static_cast<T>(0); // clang-format on
    }

    // clang-format off
    /**
     * 9-element constructor
     * @param _a
     * @param _b
     * @param _c
     * @param _d
     * @param _e
     * @param _f
     * @param _g
     * @param _h
     * @param _i
     */
    mat3(const T& _a, const T& _b, const T& _c, 
         const T& _d, const T& _e, const T& _f, 
         const T& _g, const T& _h, const T& _i)
    {
        values[0][0] = _a; values[0][1] = _b; values[0][2] =_c;
        values[1][0] = _d; values[1][1] = _e; values[1][2] =_f;
        values[2][0] = _g; values[2][1] = _h; values[2][2] =_i; // clang-format on
    }

    /**
     * mat3(A, B, C) -> [A.x, B.x, C.x, A.y, B.y, C.y, A.z, B.z, C.z]
     * @param A first column vector
     * @param B second column vector
     * @param C third column vector
     */
    mat3(const vec3<T>& A, const vec3<T>& B, const vec3<T>& C) {
        // clang-format off
        values[0][0] = A.x; values[0][1] = A.y; values[0][2] = A.z;
        values[1][0] = B.x; values[1][1] = B.y; values[1][2] = B.z;
        values[2][0] = C.x; values[2][1] = C.y; values[2][2] = C.z; // clang-format on
    }

    /**
     * Constructor from pointer of values
     * @param vals array of arrays of values
     */
    explicit mat3(T* vals) {
        values[0][0] = vals[0][0];
        values[0][2] = vals[0][1];
        values[0][1] = vals[0][2];
        values[1][0] = vals[1][0];
        values[1][2] = vals[1][1];
        values[1][1] = vals[1][2];
        values[2][0] = vals[2][0];
        values[2][2] = vals[2][1];
        values[2][1] = vals[2][2];
    }

    /**
     * default destructor
     */
    ~mat3() = default;

    /**
     * 3D-Unit-matrix generator
     * @return
     */
    static inline mat3<T> Unit() {
        return mat3<T>( // clang-format off
            static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), 
            static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), 
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)); // clang-format on
    }

    /**
     * Getter for transposed matrix
     * @return
     */
    inline mat3<T> Transpose() {
        // clang-format off
        return mat3<T>(
            values[0][0], values[1][0], values[2][0], 
            values[0][1], values[1][1], values[2][1], 
            values[0][2], values[1][2], values[2][2]); // clang-format on
    }

    /**
     * Potentially calculates the inverse matrix
     * @return inverse (A -> A^-1)
     */
    inline mat3<T> Inverse() {
        auto invDet = 1.0f / Determinant();
        return invDet * Transpose();
    }

    /**
     * test for symmetry
     * @return
     */
    inline bool IsSymmetric() {
        return values[0][1] == values[1][0] && values[0][2] == values[2][0] && values[1][2] == values[2][1];
    }

    /**
     * calculates determinant
     * @return
     */
    inline float Determinant() {
        return // clang-format off
          values[0][0] * values[1][1] * values[2][2] + values[0][1] * values[1][2] * values[2][0]
        + values[0][2] * values[1][0] * values[2][1] - values[0][2] * values[1][1] * values[2][0]
        - values[2][1] * values[1][0] * values[2][2] - values[0][0] * values[1][2] * values[2][1]; // clang-format on
    }

    /**
     * matrix-matrix addition
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat3<T> operator+(mat3<T> lhs, const mat3<T>& rhs) { return lhs += rhs; }

    /**
     * matrix-matrix subtraction
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat3<T> operator-(mat3<T> lhs, const mat3<T>& rhs) { return lhs -= rhs; }
    /**
     * Matrix-scalar multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat3<T> operator*(mat3<T> lhs, const T& rhs) { return lhs *= rhs; }

    /**
     * matrix-matrix multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec3<T> operator*(mat3<T> lhs, const vec3<T>& rhs) {
        return vec3<T>( // clang-format off
            lhs.values[0][0] * rhs.x + lhs.values[0][1] * rhs.y + lhs.values[0][2] * rhs.z, 
            lhs.values[1][0] * rhs.x + lhs.values[1][1] * rhs.y + lhs.values[1][2] * rhs.z, 
            lhs.values[2][0] * rhs.x + lhs.values[2][1] * rhs.y + lhs.values[2][2] * rhs.z // clang-format on
        );
    }

    /**
     * matrix-matrix multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat3<T> operator*(mat3<T> lhs, const mat3<T>& rhs) { return lhs *= rhs; }

    /**
     * matrix-scalar division
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat3<T> operator/(mat3<T> lhs, const T& rhs) { return lhs /= rhs; }

    /* compound assignment */
    /**
     * matrix-matrix addition
     * @param rhs
     * @return
     */
    mat3<T>& operator+=(const mat3<T>& rhs) {
        // clang-format off
        values[0][0]+=rhs[0][0]; values[0][1]+=rhs[0][1]; values[0][2]+=rhs[0][2];
        values[1][0]+=rhs[1][0]; values[1][1]+=rhs[1][1]; values[1][2]+=rhs[1][2];
        values[2][0]+=rhs[2][0]; values[2][1]+=rhs[2][1]; values[2][2]+=rhs[2][2]; // clang-format on
        return *this;
    }

    /**
     * matrix-matrix subtraction
     * @param rhs
     * @return
     */
    mat3<T>& operator-=(const mat3<T>& rhs) {
        // clang-format off
        values[0][0]-=rhs[0][0]; values[0][1]-=rhs[0][1]; values[0][2]-=rhs[0][2];
        values[1][0]-=rhs[1][0]; values[1][1]-=rhs[1][1]; values[1][2]-=rhs[1][2];
        values[2][0]-=rhs[2][0]; values[2][1]-=rhs[2][1]; values[2][2]-=rhs[2][2]; // clang-format on
        return *this;
    }

    /**
     * matrix-matrix multiplication
     * @param rhs
     * @return
     */
    mat3<T>& operator*=(const mat3<T>& rhs) {
        // clang-format off
        T _a = values[0][0], _b = values[0][1], _c = values[0][2], 
          _d = values[1][0], _e = values[1][1], _f = values[1][2], 
          _g = values[2][0], _h = values[2][1], _i = values[2][2];

        values[0][0] = _a * rhs[0][0] + _b * rhs[1][0] + _c * rhs[2][0];
        values[0][1] = _a * rhs[0][1] + _b * rhs[1][1] + _c * rhs[2][1];
        values[0][2] = _a * rhs[0][2] + _b * rhs[1][2] + _c * rhs[2][2];

        values[1][0] = _d * rhs[0][0] + _e * rhs[1][0] + _f * rhs[2][0];
        values[1][1] = _d * rhs[0][1] + _e * rhs[1][1] + _f * rhs[2][1];
        values[1][2] = _d * rhs[0][2] + _e * rhs[1][2] + _f * rhs[2][2];

        values[2][0] = _g * rhs[0][0] + _h * rhs[1][0] + _i * rhs[2][0];
        values[2][1] = _g * rhs[0][1] + _h * rhs[1][1] + _i * rhs[2][1];
        values[2][2] = _g * rhs[0][2] + _h * rhs[1][2] + _i * rhs[2][2]; // clang-format on
        return *this;
    }

    /**
     * matrix-scalar multiplication
     * @param rhs
     * @return
     */
    mat3<T>& operator*=(const T& rhs) {
        // clang-format off
        values[0][0] *= rhs; values[0][1] *= rhs; values[0][2] *= rhs; 
        values[1][0] *= rhs; values[1][1] *= rhs; values[1][2] *= rhs; 
        values[2][0] *= rhs; values[2][1] *= rhs; values[2][2] *= rhs; // clang-format on
        return *this;
    }

    /**
     * matrix-scalar division
     * @param rhs
     * @return
     */
    mat3<T>& operator/=(const T& rhs) {
        // clang-format off
        values[0][0] /= rhs; values[0][1] /= rhs; values[0][2] /= rhs;
        values[1][0] /= rhs; values[1][1] /= rhs; values[1][2] /= rhs;
        values[2][0] /= rhs; values[2][1] /= rhs; values[2][2] /= rhs; // clang-format on
        return *this;
    }

    /* Member access */
    /**
     * member access
     * @param index
     * @return
     */
    T* operator[](int index) { return values[index]; }
    /**
     * member access
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
    friend std::ostream& operator<<(std::ostream&, const mat3<U>&);
};

template<class U>
std::ostream& operator<<(std::ostream& out, const mat3<U>& mat) {
    out.precision(17);
    out << "[\n\t" // clang-format off
        << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ";\n\t" 
        << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ";\n\t" 
        << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << "\n]\n"; // clang-format on
    return out;
}

/**
 * \example mat/TestMat3.cpp
 * This is an example on how to use the mat3.h file.
 *
 */