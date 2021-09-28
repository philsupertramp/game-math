#pragma once

#include "../vec/vec4.h"
#include "mat3.h"

/**
 * 4D-matrix representation
 * @tparam T
 */
template<class T>
struct mat4 {
public:
    //! loosely packed data
    T values[4][4];

    /**
     * default constructor
     */
    mat4() {
        // clang-format off
        values[0][0] = static_cast<T>(0); values[0][1] = static_cast<T>(0); values[0][2] = static_cast<T>(0); values[0][3] = static_cast<T>(0);
        values[1][0] = static_cast<T>(0); values[1][1] = static_cast<T>(0); values[1][2] = static_cast<T>(0); values[1][3] = static_cast<T>(0);
        values[2][0] = static_cast<T>(0); values[2][1] = static_cast<T>(0); values[2][2] = static_cast<T>(0); values[2][3] = static_cast<T>(0);
        values[3][0] = static_cast<T>(0); values[3][1] = static_cast<T>(0); values[3][2] = static_cast<T>(0); values[3][3] = static_cast<T>(0); // clang-format on
    }

    // clang-format off
    /**
     * 16-element constructor
     * @param _a
     * @param _b
     * @param _c
     * @param _d
     * @param _e
     * @param _f
     * @param _g
     * @param _h
     * @param _i
     * @param _j
     * @param _k
     * @param _l
     * @param _m
     * @param _n
     * @param _o
     * @param _p
     */
    mat4(const T& _a, const T& _b, const T& _c, const T& _d, 
         const T& _e, const T& _f, const T& _g, const T& _h, 
         const T& _i, const T& _j, const T& _k, const T& _l, 
         const T& _m, const T& _n, const T& _o, const T& _p)
    {
        values[0][0] = _a; values[0][1] = _b; values[0][2] = _c; values[0][3] = _d;
        values[1][0] = _e; values[1][1] = _f; values[1][2] = _g; values[1][3] = _h;
        values[2][0] = _i; values[2][1] = _j; values[2][2] = _k; values[2][3] = _l;
        values[3][0] = _m; values[3][1] = _n; values[3][2] = _o; values[3][3] = _p; // clang-format on
    }

    /**
     * column-based constructor
     * @param A
     * @param B
     * @param C
     * @param D
     */
    mat4(const vec4<T>& A, const vec4<T>& B, const vec4<T>& C, const vec4<T>& D) {
        // clang-format off
        values[0][0] = A.x; values[0][1] = A.y; values[0][2] = A.z; values[0][3] = A.w;
        values[1][0] = B.x; values[1][1] = B.y; values[1][2] = B.z; values[1][3] = B.w;
        values[2][0] = C.x; values[2][1] = C.y; values[2][2] = C.z; values[2][3] = C.w;
        values[3][0] = D.x; values[3][1] = D.y; values[3][2] = D.z; values[3][3] = D.w; // clang-format on
    }

    /**
     * single-value constructor
     * @param _a
     */
    explicit mat4(const T& _a) {
        // clang-format off
        values[0][0] = _a; values[0][1] = static_cast<T>(0); values[0][2] = static_cast<T>(0); values[0][3] = static_cast<T>(0);
        values[1][0] = static_cast<T>(0); values[1][1] = _a; values[1][2] = static_cast<T>(0); values[1][3] = static_cast<T>(0);
        values[2][0] = static_cast<T>(0); values[2][1] = static_cast<T>(0); values[2][2] = _a; values[2][3] = static_cast<T>(0);
        values[3][0] = static_cast<T>(0); values[3][1] = static_cast<T>(0); values[3][2] = static_cast<T>(0); values[3][3] = _a; // clang-format on
    }

    /**
     * default destructor
     */
    ~mat4() = default;

    /**
     * unit-matrix generator
     * @return
     */
    static inline mat4<T> Unit() {
        return mat4<T>( // clang-format off
            static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), 
            static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), 
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), 
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)); // clang-format on
    }

    /**
     * transforms around given matrix
     * @param m
     * @return
     */
    static inline mat4<T> Transformation(const mat3<T>& m) {
        return mat4<T>( // clang-format off
            m[0][0], m[0][1], m[0][2], static_cast<T>(0), 
            m[1][0], m[1][1], m[1][2], static_cast<T>(0), 
            m[2][0], m[2][1], m[2][2], static_cast<T>(0), 
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)); // clang-format on
    }

    /**
     * getter for transposed matrix
     * @return
     */
    inline mat4<T> Transpose() {
        return mat4<T>( // clang-format off
            values[0][0], values[1][0], values[2][0], values[3][0], 
            values[0][1], values[1][1], values[2][1], values[3][1], 
            values[0][2], values[1][2], values[2][2], values[3][2], 
            values[0][3], values[1][3], values[2][3], values[3][3]); // clang-format on
    }

    /**
     * test whether matrix is symmetric
     * @return
     */
    inline bool IsSymmetric() {
        return values[0][1] == values[1][0] && values[0][2] == values[2][0] && values[0][3] == values[3][0]
               && values[2][1] == values[1][2] && values[1][3] == values[3][1] && values[3][2] == values[2][3];
    }

    /**
     * computes determinant
     * @return
     */
    inline float Determinant() {
        return // clang-format off
          values[2][0] * mat3<T>(values[0][1], values[0][2], values[0][3], values[1][1], values[1][2], values[1][3], values[2][1], values[2][2], values[2][3]).Determinant()
        - values[2][1] * mat3<T>(values[0][0], values[0][2], values[0][3], values[1][0], values[1][2], values[1][3], values[3][0], values[3][2], values[3][3]).Determinant()
        + values[2][2] * mat3<T>(values[0][0], values[0][1], values[0][3], values[1][0], values[1][1], values[1][3], values[3][0], values[3][1], values[3][3]).Determinant()
        - values[2][3] * mat3<T>(values[0][0], values[0][1], values[0][2], values[1][0], values[1][1], values[1][2], values[3][0], values[3][1], values[3][2]).Determinant(); // clang-format on
    }

    /**
     * Get minor matrix by exclusion indices
     *
     * @param i row to exclude
     * @param j column to exclude
     * @return values without row i and column j
     */
    inline mat3<T> getMinor(const size_t& i, const size_t& j) {
        mat3<T> out;
        for(size_t row = 0; row < 4; ++row) {
            for(size_t col = 0; col < 4; ++col) {
                if(row != i && col != j) out[row][col];
            }
        }
        return out;
    }

    /**
     * Calculate inverse matrix
     * @return A -> A^-1
     */
    inline mat4 Inverse() {
        T SubFactor00 = values[2][2] * values[3][3] - values[3][2] * values[2][3];
        T SubFactor01 = values[2][1] * values[3][3] - values[3][1] * values[2][3];
        T SubFactor02 = values[2][1] * values[3][2] - values[3][1] * values[2][2];
        T SubFactor03 = values[2][0] * values[3][3] - values[3][0] * values[2][3];
        T SubFactor04 = values[2][0] * values[3][2] - values[3][0] * values[2][2];
        T SubFactor05 = values[2][0] * values[3][1] - values[3][0] * values[2][1];
        T SubFactor06 = values[1][2] * values[3][3] - values[3][2] * values[1][3];
        T SubFactor07 = values[1][1] * values[3][3] - values[3][1] * values[1][3];
        T SubFactor08 = values[1][1] * values[3][2] - values[3][1] * values[1][2];
        T SubFactor09 = values[1][0] * values[3][3] - values[3][0] * values[1][3];
        T SubFactor10 = values[1][0] * values[3][2] - values[3][0] * values[1][2];
        T SubFactor11 = values[1][0] * values[3][1] - values[3][0] * values[1][1];
        T SubFactor12 = values[1][2] * values[2][3] - values[2][2] * values[1][3];
        T SubFactor13 = values[1][1] * values[2][3] - values[2][1] * values[1][3];
        T SubFactor14 = values[1][1] * values[2][2] - values[2][1] * values[1][2];
        T SubFactor15 = values[1][0] * values[2][3] - values[2][0] * values[1][3];
        T SubFactor16 = values[1][0] * values[2][2] - values[2][0] * values[1][2];
        T SubFactor17 = values[1][0] * values[2][1] - values[2][0] * values[1][1];

        mat4<T> Inverse;
        Inverse[0][0] = +(values[1][1] * SubFactor00 - values[1][2] * SubFactor01 + values[1][3] * SubFactor02);
        Inverse[0][1] = -(values[1][0] * SubFactor00 - values[1][2] * SubFactor03 + values[1][3] * SubFactor04);
        Inverse[0][2] = +(values[1][0] * SubFactor01 - values[1][1] * SubFactor03 + values[1][3] * SubFactor05);
        Inverse[0][3] = -(values[1][0] * SubFactor02 - values[1][1] * SubFactor04 + values[1][2] * SubFactor05);

        Inverse[1][0] = -(values[0][1] * SubFactor00 - values[0][2] * SubFactor01 + values[0][3] * SubFactor02);
        Inverse[1][1] = +(values[0][0] * SubFactor00 - values[0][2] * SubFactor03 + values[0][3] * SubFactor04);
        Inverse[1][2] = -(values[0][0] * SubFactor01 - values[0][1] * SubFactor03 + values[0][3] * SubFactor05);
        Inverse[1][3] = +(values[0][0] * SubFactor02 - values[0][1] * SubFactor04 + values[0][2] * SubFactor05);

        Inverse[2][0] = +(values[0][1] * SubFactor06 - values[0][2] * SubFactor07 + values[0][3] * SubFactor08);
        Inverse[2][1] = -(values[0][0] * SubFactor06 - values[0][2] * SubFactor09 + values[0][3] * SubFactor10);
        Inverse[2][2] = +(values[0][0] * SubFactor07 - values[0][1] * SubFactor09 + values[0][3] * SubFactor11);
        Inverse[2][3] = -(values[0][0] * SubFactor08 - values[0][1] * SubFactor10 + values[0][2] * SubFactor11);

        Inverse[3][0] = -(values[0][1] * SubFactor12 - values[0][2] * SubFactor13 + values[0][3] * SubFactor14);
        Inverse[3][1] = +(values[0][0] * SubFactor12 - values[0][2] * SubFactor15 + values[0][3] * SubFactor16);
        Inverse[3][2] = -(values[0][0] * SubFactor13 - values[0][1] * SubFactor15 + values[0][3] * SubFactor17);
        Inverse[3][3] = +(values[0][0] * SubFactor14 - values[0][1] * SubFactor16 + values[0][2] * SubFactor17);

        T Determinant = +values[0][0] * Inverse[0][0] + values[0][1] * Inverse[0][1] + values[0][2] * Inverse[0][2]
                        + values[0][3] * Inverse[0][3];

        Inverse /= Determinant;

        return Inverse;
    }

    /**
     * matrix-matrix addition
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat4<T> operator+(mat4<T> lhs, const mat4<T>& rhs) { return lhs += rhs; }
    /**
     * matrix-matrix subtraction
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat4<T> operator-(mat4<T> lhs, const mat4<T>& rhs) { return lhs -= rhs; }
    /**
     * matrix-matrix multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat4<T> operator*(mat4<T> lhs, const T& rhs) { return lhs *= rhs; }

    /**
     * scalar-matrix multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec4<T> operator*(mat4<T> lhs, const vec4<T>& rhs) {
        return vec4<T>( // clang-format off
            lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z + lhs[0][3] * rhs.w, 
            lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z + lhs[1][3] * rhs.w, 
            lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z + lhs[2][3] * rhs.w, 
            lhs[3][0] * rhs.x + lhs[3][1] * rhs.y + lhs[3][2] * rhs.z + lhs[3][3] * rhs.w
        ); // clang-format on
    }

    /**
     * matrix-matrix multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat4<T> operator*(mat4<T> lhs, const mat4<T>& rhs) { return lhs *= rhs; }

    /**
     * matrix-matrix division
     * @param lhs
     * @param rhs
     * @return
     */
    friend mat4<T> operator/(mat4<T> lhs, const T& rhs) { return lhs /= rhs; }

    /* compound assignment */
    /**
     * matrix-matrix addition
     * @param rhs
     * @return
     */
    mat4<T>& operator+=(const mat4<T>& rhs) {
        // clang-format off
        values[0][0] += rhs[0][0]; values[0][1] += rhs[0][1]; values[0][2] += rhs[0][2]; values[0][3] += rhs[0][3];
        values[1][0] += rhs[1][0]; values[1][1] += rhs[1][1]; values[1][2] += rhs[1][2]; values[1][3] += rhs[1][3];
        values[2][0] += rhs[2][0]; values[2][1] += rhs[2][1]; values[2][2] += rhs[2][2]; values[2][3] += rhs[2][3];
        values[3][0] += rhs[3][0]; values[3][1] += rhs[3][1]; values[3][2] += rhs[3][2]; values[3][3] += rhs[3][3]; // clang-format on
        return *this;
    }

    /**
     * matrix-matrix subtraction
     * @param rhs
     * @return
     */
    mat4<T>& operator-=(const mat4<T>& rhs) {
        // clang-format off
        values[0][0] -= rhs[0][0]; values[0][1] -= rhs[0][1]; values[0][2] -= rhs[0][2]; values[0][3] -= rhs[0][3];
        values[1][0] -= rhs[1][0]; values[1][1] -= rhs[1][1]; values[1][2] -= rhs[1][2]; values[1][3] -= rhs[1][3];
        values[2][0] -= rhs[2][0]; values[2][1] -= rhs[2][1]; values[2][2] -= rhs[2][2]; values[2][3] -= rhs[2][3];
        values[3][0] -= rhs[3][0]; values[3][1] -= rhs[3][1]; values[3][2] -= rhs[3][2]; values[3][3] -= rhs[3][3]; // clang-format on
        return *this;
    }

    /**
     * matrix-matrix multiplication
     * @param rhs
     * @return
     */
    mat4<T>& operator*=(const mat4<T>& rhs) {
        // clang-format off
        T _a = values[0][0], _b = values[0][1], _c = values[0][2], _d = values[0][3], 
          _e = values[1][0], _f = values[1][1], _g = values[1][2], _h = values[1][3], 
          _i = values[2][0], _j = values[2][1], _k = values[2][2], _l = values[2][3], 
          _m = values[3][0], _n = values[3][1], _o = values[3][2], _p = values[3][3];

        values[0][0] = _a * rhs[0][0] + _b*rhs[1][0] + _c * rhs[2][0] + _d*rhs[3][0];
        values[0][1] = _a * rhs[0][1] + _b*rhs[1][1] + _c * rhs[2][1] + _d*rhs[3][1];
        values[0][2] = _a * rhs[0][2] + _b*rhs[1][2] + _c * rhs[2][2] + _d*rhs[3][2];
        values[0][3] = _a * rhs[0][3] + _b*rhs[1][3] + _c * rhs[2][3] + _d*rhs[3][3];

        values[1][0] = _e * rhs[0][0] + _f*rhs[1][0] + _g * rhs[2][0] + _h*rhs[3][0];
        values[1][1] = _e * rhs[0][1] + _f*rhs[1][1] + _g * rhs[2][1] + _h*rhs[3][1];
        values[1][2] = _e * rhs[0][2] + _f*rhs[1][2] + _g * rhs[2][2] + _h*rhs[3][2];
        values[1][3] = _e * rhs[0][3] + _f*rhs[1][3] + _g * rhs[2][3] + _h*rhs[3][3];

        values[2][0] = _i * rhs[0][0] + _j*rhs[1][0] + _k * rhs[2][0] + _l*rhs[3][0];
        values[2][1] = _i * rhs[0][1] + _j*rhs[1][1] + _k * rhs[2][1] + _l*rhs[3][1];
        values[2][2] = _i * rhs[0][2] + _j*rhs[1][2] + _k * rhs[2][2] + _l*rhs[3][2];
        values[2][3] = _i * rhs[0][3] + _j*rhs[1][3] + _k * rhs[2][3] + _l*rhs[3][3];

        values[3][0] = _m * rhs[0][0] + _n*rhs[1][0] + _o * rhs[2][0] + _p*rhs[3][0];
        values[3][1] = _m * rhs[0][1] + _n*rhs[1][1] + _o * rhs[2][1] + _p*rhs[3][1];
        values[3][2] = _m * rhs[0][2] + _n*rhs[1][2] + _o * rhs[2][2] + _p*rhs[3][2];
        values[3][3] = _m * rhs[0][3] + _n*rhs[1][3] + _o * rhs[2][3] + _p*rhs[3][3]; // clang-format on
        return *this;
    }

    /**
     * matrix-scalar multiplication
     * @param rhs
     * @return
     */
    mat4<T>& operator*=(const T& rhs) {
        // clang-format off
        values[0][0]*=rhs; values[0][1]*=rhs; values[0][2]*=rhs; values[0][3]*=rhs;
        values[1][0]*=rhs; values[1][1]*=rhs; values[1][2]*=rhs; values[1][3]*=rhs;
        values[2][0]*=rhs; values[2][1]*=rhs; values[2][2]*=rhs; values[2][3]*=rhs;
        values[3][0]*=rhs; values[3][1]*=rhs; values[3][2]*=rhs; values[3][3]*=rhs; // clang-format on
        return *this;
    }

    /**
     * matrix-scalar division
     * @param rhs
     * @return
     */
    mat4<T>& operator/=(const T& rhs) {
        // clang-format off
        values[0][0] /= rhs; values[0][1] /= rhs; values[0][2] /= rhs; values[0][3] /= rhs;
        values[1][0] /= rhs; values[1][1] /= rhs; values[1][2] /= rhs; values[1][3] /= rhs;
        values[2][0] /= rhs; values[2][1] /= rhs; values[2][2] /= rhs; values[2][3] /= rhs;
        values[3][0] /= rhs; values[3][1] /= rhs; values[3][2] /= rhs; values[3][3] /= rhs; // clang-format on
        return *this;
    }

    /**
     * equality comparison operator
     * @param rhs
     * @return
     */
    bool operator==(const mat4<T>& rhs) {
        // clang-format off
        return (
            values[0][0] == rhs[0][0] && values[0][1] == rhs[0][1] && values[0][2] == rhs[0][2] && values[0][3] == rhs[0][3] &&
            values[1][0] == rhs[1][0] && values[1][1] == rhs[1][1] && values[1][2] == rhs[1][2] && values[1][3] == rhs[1][3] &&
            values[2][0] == rhs[2][0] && values[2][1] == rhs[2][1] && values[2][2] == rhs[2][2] && values[2][3] == rhs[2][3] &&
            values[3][0] == rhs[3][0] && values[3][1] == rhs[3][1] && values[3][2] == rhs[3][2] && values[3][3] == rhs[3][3]); // clang-format on
    }
    /**
     * not equal operator
     * @param rhs
     * @return
     */
    bool operator!=(const mat4<T>& rhs) { return !(*this == rhs); }

    /* Member access */
    /**
     * member access
     * @param index
     * @return
     */
    T* operator[](int index) { return values[index]; }
    /**
     * const member access
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
    friend std::ostream& operator<<(std::ostream&, const mat4<U>&);
};

template<class U>
std::ostream& operator<<(std::ostream& out, const mat4<U>& mat) {
    out.precision(17);
    out << "[\n\t" // clang-format off
        << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", " << mat[0][3] << ";\n\t"
        << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", " << mat[1][3] << ";\n\t"
        << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ", " << mat[2][3] << ";\n\t"
        << mat[3][0] << ", " << mat[3][1] << ", " << mat[3][2] << ", " << mat[3][3] << "\n]\n"; // clang-format on
    return out;
}

/**
 * \example mat/TestMat4.cpp
 * This is an example on how to use the mat4.h file.
 *
 */