#pragma once

#include "../vec/vec2.h"

template<class T>
struct mat2
{
public:
    T values[2][2];

    /**
     * default constructor, initializes null matrix
     */
    mat2()
    {
        values[0][0] = static_cast<T>(0);
        values[0][1] = static_cast<T>(0);
        values[1][0] = static_cast<T>(0);
        values[1][1] = static_cast<T>(0);
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
        values[0][0] = _a;
        values[0][1] = _b;
        values[1][0] = _c;
        values[1][1] = _d;
    }
    /**
     * mat2(A,B) -> [A.x, B.x, A.y, B.y]
     * @param A first column vector
     * @param B second column vector
     */
    mat2(vec2<T> A, vec2<T> B)
    {
        values[0][0] = A.x;
        values[0][1] = B.x;
        values[1][0] = A.y;
        values[1][1] = B.y;
    }
    ~mat2(){
    }

    static inline mat2<T> Unit(){ return mat2<T>(static_cast<T>(1),static_cast<T>(0),static_cast<T>(0),static_cast<T>(1)); }

    inline mat2<T> Transpose(){ return mat2<T>(values[0][0],values[1][0],values[0][1],values[1][1]); }

    inline bool IsSymmetric(){ return values[1][0] == values[0][1]; }

    inline float Determinant(){ return values[0][0]*values[1][1]-values[0][1]*values[1][0]; }

    friend mat2<T> operator+(mat2<T> lhs, const mat2<T>& rhs){ return lhs += rhs; }
    friend mat2<T> operator-(mat2<T> lhs, const mat2<T>& rhs){ return lhs -= rhs; }
    friend mat2<T> operator*(mat2<T> lhs, const T& rhs){ return lhs *= rhs; }
    friend vec2<T> operator*(mat2<T> lhs, const vec2<T>& rhs){
        return vec2<T>(
            lhs[0][0] * rhs.x + lhs[0][1] * rhs.y,
            lhs[1][0] * rhs.x + lhs[1][1] * rhs.y
        );
    }
    friend mat2<T> operator*(mat2<T> lhs, const mat2<T>& rhs){return lhs *= rhs;}
    friend mat2<T> operator/(mat2<T> lhs, const T& rhs){ return lhs /= rhs; }


    /* compound assignment */
    mat2<T>& operator+=(const mat2<T>& rhs){
        values[0][0]+=rhs[0][0]; values[0][1]+=rhs[0][1];
        values[1][0]+=rhs[1][0]; values[1][1]+=rhs[1][1];
        return *this;
    }
    mat2<T>& operator-=(const mat2<T>& rhs){
        values[0][0]-=rhs[0][0]; values[0][1]-=rhs[0][1];
        values[1][0]-=rhs[1][0]; values[1][1]-=rhs[1][1];
        return *this;
    }
    mat2<T>& operator*=(const mat2<T>& rhs){
        T _a = values[0][0], _b = values[0][1], _c = values[1][0], _d = values[1][1];
        values[0][0] = _a * rhs[0][0] + _b * rhs[1][0]; values[0][1] = _a * rhs[0][1] + _b * rhs[1][1];
        values[1][0] = _c * rhs[0][0] + _d * rhs[1][0]; values[1][1] = _c * rhs[0][1] + _d * rhs[1][1];
        return *this;
    }
    mat2<T>& operator*=(const T& rhs){
        values[0][0]*=rhs; values[0][1]*=rhs;
        values[1][0]*=rhs; values[1][1]*=rhs;
        return *this;
    }
    mat2<T>& operator/=(const T& rhs){
        values[0][0]/=rhs; values[0][1]/=rhs;
        values[1][0]/=rhs; values[1][1]/=rhs;
        return *this;
    }

    /* Member access */
    T* operator[](int index){
        return values[index];
    }
    const T* operator[](int index) const {
        return values[index];
    }
    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const mat2<U>&);
};

template<class U>
std::ostream &operator<<(std::ostream &out, const mat2<U> &mat) {
    out << "[\n\t" << mat[0][0] << ", " << mat[0][1] << ";\n\t" << mat[1][0] << ", " << mat[1][1] << "\n]\n";
    return out;
}
