#pragma once

#include "../vec/vec4.h"
#include "mat3.h"

template<class T>
struct mat4
{

    typedef vec4<T> col_type;
    
public:

    col_type values[4];

    mat4(){
        values[0][0] = static_cast<T>(0); values[0][1] = static_cast<T>(0); values[0][2] = static_cast<T>(0); values[0][3] = static_cast<T>(0);
        values[1][0] = static_cast<T>(0); values[1][1] = static_cast<T>(0); values[1][2] = static_cast<T>(0); values[1][3] = static_cast<T>(0);
        values[2][0] = static_cast<T>(0); values[2][1] = static_cast<T>(0); values[2][2] = static_cast<T>(0); values[2][3] = static_cast<T>(0);
        values[3][0] = static_cast<T>(0); values[3][1] = static_cast<T>(0); values[3][2] = static_cast<T>(0); values[3][3] = static_cast<T>(0);
    }
    mat4(T _a, T _b, T _c, T _d, T _e, T _f, T _g, T _h, T _i, T _j, T _k, T _l, T _m, T _n, T _o, T _p){
        values[0][0] = _a; values[0][1] = _b; values[0][2] = _c; values[0][3] = _d;
        values[1][0] = _e; values[1][1] = _f; values[1][2] = _g; values[1][3] = _h;
        values[2][0] = _i; values[2][1] = _j; values[2][2] = _k; values[2][3] = _l;
        values[3][0] = _m; values[3][1] = _n; values[3][2] = _o; values[3][3] = _p;
    }
    mat4(vec4<T> A, vec4<T> B, vec4<T> C, vec4<T> D){
        values[0][0] = A.x; values[0][1] = A.y; values[0][2] = A.z; values[0][3] = A.w;
        values[1][0] = B.x; values[1][1] = B.y; values[1][2] = B.z; values[1][3] = B.w;
        values[2][0] = C.x; values[2][1] = C.y; values[2][2] = C.z; values[2][3] = C.w;
        values[3][0] = D.x; values[3][1] = D.y; values[3][2] = D.z; values[3][3] = D.w;
    }
    mat4(T _a){
        values[0][0] = _a; values[0][1] = static_cast<T>(0); values[0][2] = static_cast<T>(0); values[0][3] = static_cast<T>(0);
        values[1][0] = static_cast<T>(0); values[1][1] = _a; values[1][2] = static_cast<T>(0); values[1][3] = static_cast<T>(0);
        values[2][0] = static_cast<T>(0); values[2][1] = static_cast<T>(0); values[2][2] = _a; values[2][3] = static_cast<T>(0);
        values[3][0] = static_cast<T>(0); values[3][1] = static_cast<T>(0); values[3][2] = static_cast<T>(0); values[3][3] = _a;
    }
    ~mat4(){
    }

    static inline mat4<T> Unit(){
        return mat4<T>(
                static_cast<T>(1),static_cast<T>(0),static_cast<T>(0),static_cast<T>(0),
                static_cast<T>(0),static_cast<T>(1),static_cast<T>(0),static_cast<T>(0),
                static_cast<T>(0),static_cast<T>(0),static_cast<T>(1),static_cast<T>(0),
                static_cast<T>(0),static_cast<T>(0),static_cast<T>(0),static_cast<T>(1));
    }

    static inline mat4<T> Transformation(mat3<T> m){
        return mat4<T>(
                m[0][0],m[0][1],m[0][2],static_cast<T>(0),
                m[1][0],m[1][1],m[1][2],static_cast<T>(0),
                m[2][0],m[2][1],m[2][2],static_cast<T>(0),
                static_cast<T>(  0),static_cast<T>(  0),static_cast<T>(  0),static_cast<T>(1));
    }

    inline mat4<T> Transpose(){
        return mat4<T>(
                values[0][0],values[1][0],values[2][0],values[3][0],
                values[0][1],values[1][1],values[2][1],values[3][1],
                values[0][2],values[1][2],values[2][2],values[3][2],
                values[0][3],values[1][3],values[2][3],values[3][3]);
    }

    inline bool IsSymmetric(){
        return values[0][1]==values[1][0]&&values[0][2]==values[2][0]&&values[0][3]==values[3][0]&&values[2][1]==values[1][2]&&values[1][3]==values[3][1]&&values[3][2]==values[2][3];
    }

    inline float Determinant(){
        return values[2][0] * mat3<T>(values[0][1],values[0][2],values[0][3],values[1][1],values[1][2],values[1][3],values[2][1],values[2][2],values[2][3]).Determinant()
        - values[2][1] * mat3<T>(values[0][0],values[0][2],values[0][3],values[1][0],values[1][2],values[1][3],values[3][0],values[3][2],values[3][3]).Determinant()
        + values[2][2] * mat3<T>(values[0][0],values[0][1],values[0][3],values[1][0],values[1][1],values[1][3],values[3][0],values[3][1],values[3][3]).Determinant()
        - values[2][3] * mat3<T>(values[0][0],values[0][1],values[0][2],values[1][0],values[1][1],values[1][2],values[3][0],values[3][1],values[3][2]).Determinant() ;
    }


    friend mat4<T> operator+(mat4<T> lhs, const mat4<T>& rhs){ return lhs += rhs; }
    friend mat4<T> operator-(mat4<T> lhs, const mat4<T>& rhs){ return lhs -= rhs; }
    friend mat4<T> operator*(mat4<T> lhs, const T& rhs){ return lhs *= rhs; }
    friend vec4<T> operator*(mat4<T> lhs, const vec4<T>& rhs){
        return vec4<T>(
                lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z + lhs[0][3] * rhs.w,
                lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z + lhs[1][3] * rhs.w,
                lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z + lhs[2][3] * rhs.w,
                lhs[3][0] * rhs.x + lhs[3][1] * rhs.y + lhs[3][2] * rhs.z + lhs[3][3] * rhs.w
        );
    }
    friend mat4<T> operator*(mat4<T> lhs, const mat4<T>& rhs){return lhs *= rhs;}
    friend mat4<T> operator/(mat4<T> lhs, const T& rhs){ return lhs /= rhs; }


    /* compound assignment */
    mat4<T>& operator+=(const mat4<T>& rhs){
        values[0][0]+=rhs[0][0]; values[0][1]+=rhs[0][1]; values[0][2]+=rhs[0][2]; values[0][3]+=rhs[0][3];
        values[1][0]+=rhs[1][0]; values[1][1]+=rhs[1][1]; values[1][2]+=rhs[1][2]; values[1][3]+=rhs[1][3];
        values[2][0]+=rhs[2][0]; values[2][1]+=rhs[2][1]; values[2][2]+=rhs[2][2]; values[2][3]+=rhs[2][3];
        values[3][0]+=rhs[3][0]; values[3][1]+=rhs[3][1]; values[3][2]+=rhs[3][2]; values[3][3]+=rhs[3][3];
        return *this;
    }
    mat4<T>& operator-=(const mat4<T>& rhs){
        values[0][0]-=rhs[0][0]; values[0][1]-=rhs[0][1]; values[0][2]-=rhs[0][2]; values[0][3]-=rhs[0][3];
        values[1][0]-=rhs[1][0]; values[1][1]-=rhs[1][1]; values[1][2]-=rhs[1][2]; values[1][3]-=rhs[1][3];
        values[2][0]-=rhs[2][0]; values[2][1]-=rhs[2][1]; values[2][2]-=rhs[2][2]; values[2][3]-=rhs[2][3];
        values[3][0]-=rhs[3][0]; values[3][1]-=rhs[3][1]; values[3][2]-=rhs[3][2]; values[3][3]-=rhs[3][3];
        return *this;
    }
    mat4<T>& operator*=(const mat4<T>& rhs){
        T _a = values[0][0], _b = values[0][1], _c = values[0][2], _d = values[0][3], _e = values[1][0],
                _f=values[1][1],_g=values[1][2],_h=values[1][3],_i=values[2][0],_j=values[2][1],_k=values[2][2],_l=values[2][3],_m=values[3][0],
                _n=values[3][1],_o=values[3][2],_p=values[3][3];
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
        values[3][3] = _m * rhs[0][3] + _n*rhs[1][3] + _o * rhs[2][3] + _p*rhs[3][3];

        return *this;
    }
    mat4<T>& operator*=(const T& rhs){
        values[0][0]*=rhs; values[0][1]*=rhs; values[0][2]*=rhs; values[0][3]*=rhs;
        values[1][0]*=rhs; values[1][1]*=rhs; values[1][2]*=rhs; values[1][3]*=rhs;
        values[2][0]*=rhs; values[2][1]*=rhs; values[2][2]*=rhs; values[2][3]*=rhs;
        values[3][0]*=rhs; values[3][1]*=rhs; values[3][2]*=rhs; values[3][3]*=rhs;
        return *this;
    }
    mat4<T>& operator/=(const T& rhs){
        values[0][0]/=rhs; values[0][1]/=rhs; values[0][2]/=rhs; values[0][3]/=rhs;
        values[1][0]/=rhs; values[1][1]/=rhs; values[1][2]/=rhs; values[1][3]/=rhs;
        values[2][0]/=rhs; values[2][1]/=rhs; values[2][2]/=rhs; values[2][3]/=rhs;
        values[3][0]/=rhs; values[3][1]/=rhs; values[3][2]/=rhs; values[3][3]/=rhs;
        return *this;
    }

    /* Member access */
    col_type& operator[](int index){
        return values[index];
    }
    const col_type& operator[](int index) const {
        return values[index];
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const mat4<U>&);
};

template<class U>
std::ostream &operator<<(std::ostream &out, const mat4 <U> &mat) {
    out << "[\n\t" << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", " << mat[0][3] << ";\n\t"
        << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", " << mat[1][3] << ";\n\t"
        << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ", " << mat[2][3] << ";\n\t"
        << mat[3][0] << ", " << mat[3][1] << ", " << mat[3][2] << ", " << mat[3][3] << "\n]\n";
    return out;
}
