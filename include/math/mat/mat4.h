#pragma once

#include "../vec/vec4.h"
#include "mat3.h"

template<class T>
class mat4
{
    T** values[4];

public:
    T a,b,c,d, e,f,g,h, i,j,k,l, m,n,o,p;

    mat4(){
        values[0] = new T*[4]; values[1] = new T*[4];
        values[2] = new T*[4]; values[3] = new T*[4];
        a = static_cast<T>(0);  b = static_cast<T>(0);  c = static_cast<T>(0);  d = static_cast<T>(0);
        e = static_cast<T>(0);  f = static_cast<T>(0);  g = static_cast<T>(0);  h = static_cast<T>(0);
        i = static_cast<T>(0);  j = static_cast<T>(0);  k = static_cast<T>(0);  l = static_cast<T>(0);
        m = static_cast<T>(0);  n = static_cast<T>(0);  o = static_cast<T>(0);  p = static_cast<T>(0);
        
        values[0][0] = &a; values[0][1] = &b; values[0][2] = &c; values[0][3] = &d;
        values[1][0] = &e; values[1][1] = &f; values[1][2] = &g; values[1][3] = &h;
        values[2][0] = &i; values[2][1] = &j; values[2][2] = &k; values[2][3] = &l;
        values[3][0] = &m; values[3][1] = &n; values[3][2] = &o; values[3][3] = &p;
    }
    mat4(T _a, T _b, T _c, T _d, T _e, T _f, T _g, T _h, T _i, T _j, T _k, T _l, T _m, T _n, T _o, T _p){
        values[0] = new T*[4]; values[1] = new T*[4];
        values[2] = new T*[4]; values[3] = new T*[4];
        a = _a;  b = _b;  c = _c;  d = _d;
        e = _e;  f = _f;  g = _g;  h = _h;
        i = _i;  j = _j;  k = _k;  l = _l;
        m = _m;  n = _n;  o = _o;  p = _p;

        values[0][0] = &a; values[0][1] = &b; values[0][2] = &c; values[0][3] = &d;
        values[1][0] = &e; values[1][1] = &f; values[1][2] = &g; values[1][3] = &h;
        values[2][0] = &i; values[2][1] = &j; values[2][2] = &k; values[2][3] = &l;
        values[3][0] = &m; values[3][1] = &n; values[3][2] = &o; values[3][3] = &p;
    }
    mat4(vec4<T> A, vec4<T> B, vec4<T> C, vec4<T> D){
        values[0] = new T*[4]; values[1] = new T*[4];
        values[2] = new T*[4]; values[3] = new T*[4];
        a = A.x;  b = A.y;  c = A.z;  d = A.w;
        e = B.x;  f = B.y;  g = B.z;  h = B.w;
        i = C.x;  j = C.y;  k = C.z;  l = C.w;
        m = D.x;  n = D.y;  o = D.z;  p = D.w;

        values[0][0] = &a; values[0][1] = &b; values[0][2] = &c; values[0][3] = &d;
        values[1][0] = &e; values[1][1] = &f; values[1][2] = &g; values[1][3] = &h;
        values[2][0] = &i; values[2][1] = &j; values[2][2] = &k; values[2][3] = &l;
        values[3][0] = &m; values[3][1] = &n; values[3][2] = &o; values[3][3] = &p;
    }
    mat4(T _a){
        values[0] = new T*[4]; values[1] = new T*[4];
        values[2] = new T*[4]; values[3] = new T*[4];
        a = _a;  b = 0;  c = 0;  d = 0;
        e = 0;  f = _a;  g = 0;  h = 0;
        i = 0;  j = 0;  k = _a;  l = 0;
        m = 0;  n = 0;  o = 0;  p = _a;

        values[0][0] = &a; values[0][1] = &b; values[0][2] = &c; values[0][3] = &d;
        values[1][0] = &e; values[1][1] = &f; values[1][2] = &g; values[1][3] = &h;
        values[2][0] = &i; values[2][1] = &j; values[2][2] = &k; values[2][3] = &l;
        values[3][0] = &m; values[3][1] = &n; values[3][2] = &o; values[3][3] = &p;
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
                static_cast<T>(m.a),static_cast<T>(m.b),static_cast<T>(m.c),static_cast<T>(0),
                static_cast<T>(m.d),static_cast<T>(m.e),static_cast<T>(m.f),static_cast<T>(0),
                static_cast<T>(m.g),static_cast<T>(m.h),static_cast<T>(m.i),static_cast<T>(0),
                static_cast<T>(  0),static_cast<T>(  0),static_cast<T>(  0),static_cast<T>(1));
    }

    inline mat4<T> Transpose(){ return mat4<T>(a,e,i,m,b,f,j,n,c,g,k,o,d,h,l,p); }

    inline bool IsSymmetric(){ return b==e&&c==i&&d==m&&j==g&&h==n&&o==l; }

    inline float Determinant(){
        return i * mat3<T>(b,c,d,f,g,h,n,o,p).Determinant() - j * mat3<T>(a,c,d,e,g,h,m,o,p).Determinant() + k*mat3<T>(a,b,d,e,f,h,m,n,p).Determinant() - l * mat3<T>(a,b,c,e,f,g,m,n,o).Determinant() ;
    }


    friend mat4<T> operator+(mat4<T> lhs, const mat4<T>& rhs){ return lhs += rhs; }
    friend mat4<T> operator-(mat4<T> lhs, const mat4<T>& rhs){ return lhs -= rhs; }
    friend mat4<T> operator*(mat4<T> lhs, const T& rhs){ return lhs *= rhs; }
    friend vec4<T> operator*(mat4<T> lhs, const vec4<T>& rhs){
        return vec4<T>(
                lhs.a * rhs.x + lhs.b * rhs.y + lhs.c * rhs.z + lhs.d * rhs.w,
                lhs.e * rhs.x + lhs.f * rhs.y + lhs.g * rhs.z + lhs.h * rhs.w,
                lhs.i * rhs.x + lhs.j * rhs.y + lhs.k * rhs.z + lhs.l * rhs.w,
                lhs.m * rhs.x + lhs.n * rhs.y + lhs.o * rhs.z + lhs.p * rhs.w
        );
    }
    friend mat4<T> operator*(mat4<T> lhs, const mat4<T>& rhs){return lhs *= rhs;}
    friend mat4<T> operator/(mat4<T> lhs, const T& rhs){ return lhs /= rhs; }


    /* compound assignment */
    mat4<T>& operator+=(const mat4<T>& rhs){
        a+=rhs.a; b+=rhs.b; c+=rhs.c; d+=rhs.d;
        e+=rhs.e; f+=rhs.f; g+=rhs.g; h+=rhs.h;
        i+=rhs.i; j+=rhs.j; k+=rhs.k; l+=rhs.l;
        m+=rhs.m; n+=rhs.n; o+=rhs.o; p+=rhs.p;
        return *this;
    }
    mat4<T>& operator-=(const mat4<T>& rhs){
        a-=rhs.a; b-=rhs.b; c-=rhs.c; d-=rhs.d;
        e-=rhs.e; f-=rhs.f; g-=rhs.g; h-=rhs.h;
        i-=rhs.i; j-=rhs.j; k-=rhs.k; l-=rhs.l;
        m-=rhs.m; n-=rhs.n; o-=rhs.o; p-=rhs.p;
        return *this;
    }
    mat4<T>& operator*=(const mat4<T>& rhs){
        T _a = a, _b = b, _c = c, _d = d, _e = e, _f=f,_g=g,_h=h,_i=i,_j=j,_k=k,_l=l,_m=m,_n=n,_o=o,_p=p;
        a = _a * rhs.a + _b*rhs.e + _c * rhs.i + _d*rhs.m;
        b = _a * rhs.b + _b*rhs.f + _c * rhs.j + _d*rhs.n;
        c = _a * rhs.c + _b*rhs.g + _c * rhs.k + _d*rhs.o;
        d = _a * rhs.d + _b*rhs.h + _c * rhs.l + _d*rhs.p;

        e = _e * rhs.a + _f*rhs.e + _g * rhs.i + _h*rhs.m;
        f = _e * rhs.b + _f*rhs.f + _g * rhs.j + _h*rhs.n;
        g = _e * rhs.c + _f*rhs.g + _g * rhs.k + _h*rhs.o;
        h = _e * rhs.d + _f*rhs.h + _g * rhs.l + _h*rhs.p;

        i = _i * rhs.a + _j*rhs.e + _k * rhs.i + _l*rhs.m;
        j = _i * rhs.b + _j*rhs.f + _k * rhs.j + _l*rhs.n;
        k = _i * rhs.c + _j*rhs.g + _k * rhs.k + _l*rhs.o;
        l = _i * rhs.d + _j*rhs.h + _k * rhs.l + _l*rhs.p;

        m = _m * rhs.a + _n*rhs.e + _o * rhs.i + _p*rhs.m;
        n = _m * rhs.b + _n*rhs.f + _o * rhs.j + _p*rhs.n;
        o = _m * rhs.c + _n*rhs.g + _o * rhs.k + _p*rhs.o;
        p = _m * rhs.d + _n*rhs.h + _o * rhs.l + _p*rhs.p;

        return *this;
    }
    mat4<T>& operator*=(const T& rhs){
        a*=rhs; b*=rhs; c*=rhs; d*=rhs;
        e*=rhs; f*=rhs; g*=rhs; h*=rhs;
        i*=rhs; j*=rhs; k*=rhs; l*=rhs;
        m*=rhs; n*=rhs; o*=rhs; p*=rhs;
        return *this;
    }
    mat4<T>& operator/=(const T& rhs){
        a/=rhs; b/=rhs; c/=rhs; d/=rhs;
        e/=rhs; f/=rhs; g/=rhs; h/=rhs;
        i/=rhs; j/=rhs; k/=rhs; l/=rhs;
        m/=rhs; n/=rhs; o/=rhs; p/=rhs;
        return *this;
    }

    /* Member access */
    T*& operator[](int index){
        return values[index];
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const mat4<U>&);
};

template<class U>
std::ostream &operator<<(std::ostream &out, const mat4 <U> &mat) {
    out << "[\n\t" << mat.a << ", " << mat.b << ", " << mat.c << ", " << mat.d << ";\n\t"
        << mat.e << ", " << mat.f << ", " << mat.g << ", " << mat.h << ";\n\t"
        << mat.i << ", " << mat.j << ", " << mat.k << ", " << mat.l << ";\n\t"
        << mat.m << ", " << mat.n << ", " << mat.o << ", " << mat.p << "\n]\n";
    return out;
}
