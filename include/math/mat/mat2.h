#pragma once

#include "../vec/vec2.h"

template<class T>
class mat2
{
    T* values[2];
public:
    T a, b, c, d;

    /**
     * default constructor, initializes null matrix
     */
    mat2()
    {
        values[0] = new T[2];
        values[1] = new T[2];
        a = static_cast<T>(0); values[0][0] = a;
        b = static_cast<T>(0); values[0][1] = b;
        c = static_cast<T>(0); values[1][0] = c;
        d = static_cast<T>(0); values[1][1] = d;
    }
    /**
     * mat2(a,b,c,d) -> [a,b,c,d]
     * @param _a  m_11
     * @param _b  m_12
     * @param _c  m_21
     * @param _d  m_22
     */
    mat2(T _a, T _b, T _c, T _d)
    {
        values[0] = new T[2];
        values[1] = new T[2];
        a = _a; values[0][0] = a;
        b = _b; values[0][1] = b;
        c = _c; values[1][0] = c;
        d = _d; values[1][1] = d;
    }
    /**
     * mat2(A,B) -> [A.x, B.x, A.y, B.y]
     * @param A first column vector
     * @param B second column vector
     */
    mat2(vec2<T> A, vec2<T> B)
    {
        values[0] = new T*[2];
        values[1] = new T*[2];
        a = A.x; values[0][0] = a;
        b = B.x; values[0][1] = b;
        c = A.y; values[1][0] = c;
        d = B.y; values[1][1] = d;
    }
    ~mat2(){
    }

    static inline mat2<T> Unit(){ return mat2<T>(static_cast<T>(1),static_cast<T>(0),static_cast<T>(0),static_cast<T>(1)); }

    inline mat2<T> Transpose(){ return mat2<T>(a,c,b,d); }

    inline bool IsSymmetric(){ return b == c; }

    inline float Determinant(){ return a*d-b*c; }

    friend mat2<T> operator+(mat2<T> lhs, const mat2<T>& rhs){ return lhs += rhs; }
    friend mat2<T> operator-(mat2<T> lhs, const mat2<T>& rhs){ return lhs -= rhs; }
    friend mat2<T> operator*(mat2<T> lhs, const T& rhs){ return lhs *= rhs; }
    friend vec2<T> operator*(mat2<T> lhs, const vec2<T>& rhs){
        return vec2<T>(
            lhs.a * rhs.x + lhs.b * rhs.y,
            lhs.c * rhs.x + lhs.d * rhs.y
        );
    }
    friend mat2<T> operator*(mat2<T> lhs, const mat2<T>& rhs){return lhs *= rhs;}
    friend mat2<T> operator/(mat2<T> lhs, const T& rhs){ return lhs /= rhs; }


    /* compound assignment */
    mat2<T>& operator+=(const mat2<T>& rhs){
        a+=rhs.a; b+=rhs.b;
        c+=rhs.c; d+=rhs.d;
        return *this;
    }
    mat2<T>& operator-=(const mat2<T>& rhs){
        a-=rhs.a; b-=rhs.b;
        c-=rhs.c; d-=rhs.d;
        return *this;
    }
    mat2<T>& operator*=(const mat2<T>& rhs){
        T _a = a, _b = b, _c = c, _d = d;
        a = _a * rhs.a + _b * rhs.c; b = _a * rhs.b + _b * rhs.d;
        c = _c * rhs.a + _d * rhs.c; d = _c * rhs.b + _d * rhs.d;
        return *this;
    }
    mat2<T>& operator*=(const T& rhs){
        a*=rhs; b*=rhs;
        c*=rhs; d*=rhs;
        return *this;
    }
    mat2<T>& operator/=(const T& rhs){
        a/=rhs; b/=rhs;
        c/=rhs; d/=rhs;
        return *this;
    }

    /* Member access */
    T** operator[](int index){
        return values[index];
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const mat2<U>&);
};

template<class U>
std::ostream &operator<<(std::ostream &out, const mat2<U> &mat) {
    out << "[\n\t" << mat.a << ", " << mat.b << ";\n\t" << mat.c << ", " << mat.d << "\n]\n";
    return out;
}
