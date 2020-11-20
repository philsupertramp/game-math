#pragma once

#include "../vec/vec3.h"

template<class T>
struct mat3
{
public:
    T values[3][3];

    /**
     * default constructor, initializes null matrix
     */
    mat3()
    {
        values[0][0] = static_cast<T>(0);
        values[0][1] = static_cast<T>(0);
        values[0][2] = static_cast<T>(0);
        values[1][0] = static_cast<T>(0);
        values[1][1] = static_cast<T>(0);
        values[1][2] = static_cast<T>(0);
        values[2][0] = static_cast<T>(0);
        values[2][1] = static_cast<T>(0);
        values[2][2] = static_cast<T>(0);
    }
    /**
     * mat3(a,b,c,d) -> [a,b,c,d]
     * @param _a  m_11
     * @param _b  m_12
     * @param _c  m_21
     * @param _d  m_22
     */
    mat3(T _a, T _b, T _c, T _d, T _e, T _f, T _g, T _h, T _i)
    {
        values[0][0] = _a;
        values[0][1] = _b;
        values[0][2] = _c;
        values[1][0] = _d;
        values[1][1] = _e;
        values[1][2] = _f;
        values[2][0] = _g;
        values[2][1] = _h;
        values[2][2] = _i;
    }
    /**
     * mat3(A,B) -> [A.x, B.x, A.y, B.y]
     * @param A first column vector
     * @param B second column vector
     */
    mat3(vec3<T> A, vec3<T> B, vec3<T> C)
    {
        values[0][0] = A.x;
        values[0][1] = A.y;
        values[0][2] = A.z;
        values[1][0] = B.x;
        values[1][1] = B.y;
        values[1][2] = B.z;
        values[2][0] = C.x;
        values[2][1] = C.y;
        values[2][2] = C.z;
    }
    ~mat3() { }

    static inline mat3<T> Unit()
    {
        return mat3<T>(static_cast<T>(1),
                       static_cast<T>(0),
                       static_cast<T>(0),
                       static_cast<T>(0),
                       static_cast<T>(1),
                       static_cast<T>(0),
                       static_cast<T>(0),
                       static_cast<T>(0),
                       static_cast<T>(1));
    }

    inline mat3<T> Transpose()
    {
        return mat3<T>(values[0][0],
                       values[1][0],
                       values[2][0],
                       values[0][1],
                       values[1][1],
                       values[2][1],
                       values[0][2],
                       values[1][2],
                       values[2][2]);
    }

    inline bool IsSymmetric()
    {
        return values[0][1] == values[1][0] && values[0][2] == values[2][0] && values[1][2] == values[2][1];
    }

    inline float Determinant()
    {
        return values[0][0] * values[1][1] * values[2][2] + values[0][1] * values[1][2] * values[2][0]
               + values[0][2] * values[1][0] * values[2][1] - values[0][2] * values[1][1] * values[2][0]
               - values[2][1] * values[1][0] * values[2][2] - values[0][0] * values[1][2] * values[2][1];
    }

    friend mat3<T> operator+(mat3<T> lhs, const mat3<T>& rhs) { return lhs += rhs; }
    friend mat3<T> operator-(mat3<T> lhs, const mat3<T>& rhs) { return lhs -= rhs; }
    friend mat3<T> operator*(mat3<T> lhs, const T& rhs) { return lhs *= rhs; }
    friend vec3<T> operator*(mat3<T> lhs, const vec3<T>& rhs)
    {
        return vec3<T>(lhs.values[0][0] * rhs.x + lhs.values[0][1] * rhs.y + lhs.values[0][2] * rhs.z,
                       lhs.values[1][0] * rhs.x + lhs.values[1][1] * rhs.y + lhs.values[1][2] * rhs.z,
                       lhs.values[2][0] * rhs.x + lhs.values[2][1] * rhs.y + lhs.values[2][2] * rhs.z);
    }
    friend mat3<T> operator*(mat3<T> lhs, const mat3<T>& rhs) { return lhs *= rhs; }
    friend mat3<T> operator/(mat3<T> lhs, const T& rhs) { return lhs /= rhs; }


    /* compound assignment */
    mat3<T>& operator+=(const mat3<T>& rhs)
    {
        values[0][0] += rhs[0][0];
        values[0][1] += rhs[0][1];
        values[0][2] += rhs[0][2];
        values[1][0] += rhs[1][0];
        values[1][1] += rhs[1][1];
        values[1][2] += rhs[1][2];
        values[2][0] += rhs[2][0];
        values[2][1] += rhs[2][1];
        values[2][2] += rhs[2][2];
        return *this;
    }
    mat3<T>& operator-=(const mat3<T>& rhs)
    {
        values[0][0] -= rhs[0][0];
        values[0][1] -= rhs[0][1];
        values[0][2] -= rhs[0][2];
        values[1][0] -= rhs[1][0];
        values[1][1] -= rhs[1][1];
        values[1][2] -= rhs[1][2];
        values[2][0] -= rhs[2][0];
        values[2][1] -= rhs[2][1];
        values[2][2] -= rhs[2][2];
        return *this;
    }
    mat3<T>& operator*=(const mat3<T>& rhs)
    {
        T _a = values[0][0], _b = values[0][1], _c = values[0][2], _d = values[1][0], _e = values[1][1],
          _f = values[1][2], _g = values[2][0], _h = values[2][1], _i = values[2][2];
        values[0][0] = _a * rhs[0][0] + _b * rhs[1][0] + _c * rhs[2][0];
        values[0][1] = _a * rhs[0][1] + _b * rhs[1][1] + _c * rhs[2][1];
        values[0][2] = _a * rhs[0][2] + _b * rhs[1][2] + _c * rhs[2][2];

        values[1][0] = _d * rhs[0][0] + _e * rhs[1][0] + _f * rhs[2][0];
        values[1][1] = _d * rhs[0][1] + _e * rhs[1][1] + _f * rhs[2][1];
        values[1][2] = _d * rhs[0][2] + _e * rhs[1][2] + _f * rhs[2][2];

        values[2][0] = _g * rhs[0][0] + _h * rhs[1][0] + _i * rhs[2][0];
        values[2][1] = _g * rhs[0][1] + _h * rhs[1][1] + _i * rhs[2][1];
        values[2][2] = _g * rhs[0][2] + _h * rhs[1][2] + _i * rhs[2][2];
        return *this;
    }
    mat3<T>& operator*=(const T& rhs)
    {
        values[0][0] *= rhs;
        values[0][1] *= rhs;
        values[0][2] *= rhs;
        values[1][0] *= rhs;
        values[1][1] *= rhs;
        values[1][2] *= rhs;
        values[2][0] *= rhs;
        values[2][1] *= rhs;
        values[2][2] *= rhs;
        return *this;
    }
    mat3<T>& operator/=(const T& rhs)
    {
        values[0][0] /= rhs;
        values[0][1] /= rhs;
        values[0][2] /= rhs;
        values[1][0] /= rhs;
        values[1][1] /= rhs;
        values[1][2] /= rhs;
        values[2][0] /= rhs;
        values[2][1] /= rhs;
        values[2][2] /= rhs;
        return *this;
    }

    /* Member access */
    T* operator[](int index) { return values[index]; }
    const T* operator[](int index) const { return values[index]; }


    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const mat3<U>&);
};

template<class U>
std::ostream& operator<<(std::ostream& out, const mat3<U>& mat)
{
    out << "[\n\t" << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ";\n\t" << mat[1][0] << ", " << mat[1][1]
        << ", " << mat[1][2] << ";\n\t" << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << "\n]\n";
    return out;
}
