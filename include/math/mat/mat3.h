#pragma once

#include "../vec/vec3.h"

template<class T>
class mat3
{
    T** values[3];
public:
    T a, b, c, d, e, f, g, h, i;

    /**
     * default constructor, initializes null matrix
     */
    mat3()
    {
        values[0] = new T*[3]; values[1] = new T*[3]; values[2] = new T*[3];
        a = static_cast<T>(0); values[0][0] = &a;   b = static_cast<T>(0); values[0][1] = &b;   c = static_cast<T>(0); values[0][2] = &c;
        d = static_cast<T>(0); values[1][0] = &d;   e = static_cast<T>(0); values[1][1] = &e;   f = static_cast<T>(0); values[1][2] = &f;
        g = static_cast<T>(0); values[2][0] = &g;   h = static_cast<T>(0); values[2][1] = &h;   i = static_cast<T>(0); values[2][2] = &i;
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
        values[0] = new T*[3]; values[1] = new T*[3]; values[2] = new T*[3];
        a = _a; values[0][0] = &a;  b = _b; values[0][1] = &b;  c = _c; values[0][2] = &c;
        d = _d; values[1][0] = &d;  e = _e; values[1][1] = &e;  f = _f; values[1][2] = &f;
        g = _g; values[2][0] = &g;  h = _h; values[2][1] = &h;  i = _i; values[2][2] = &i;
    }
    /**
     * mat3(A,B) -> [A.x, B.x, A.y, B.y]
     * @param A first column vector
     * @param B second column vector
     */
    mat3(vec3<T> A, vec3<T> B, vec3<T> C)
    {
        values[0] = new T*[3]; values[1] = new T*[3]; values[2] = new T*[3];
        a = A.x; values[0][0] = &a;  b = A.y; values[0][1] = &b;  c = A.z; values[0][2] = &c;
        d = B.x; values[1][0] = &d;  e = B.y; values[1][1] = &e;  f = B.z; values[1][2] = &f;
        g = C.x; values[2][0] = &g;  h = C.y; values[2][1] = &h;  i = C.z; values[2][2] = &i;
    }
    ~mat3(){
    }

    static inline mat3<T> Unit(){
        return mat3<T>(
            static_cast<T>(1),static_cast<T>(0),static_cast<T>(0),
            static_cast<T>(0),static_cast<T>(1),static_cast<T>(0),
            static_cast<T>(0),static_cast<T>(0),static_cast<T>(1));
    }

    inline mat3<T> Transpose(){ return mat3<T>(a,d,g,b,e,h,c,f,i); }

    inline bool IsSymmetric(){ return b==d && c == g && f == h; }

    inline float Determinant(){ return a*e*i+b*f*g+c*d*h-c*e*g-b*d*i-a*f*h; }

    friend mat3<T> operator+(mat3<T> lhs, const mat3<T>& rhs){ return lhs += rhs; }
    friend mat3<T> operator-(mat3<T> lhs, const mat3<T>& rhs){ return lhs -= rhs; }
    friend mat3<T> operator*(mat3<T> lhs, const T& rhs){ return lhs *= rhs; }
    friend vec3<T> operator*(mat3<T> lhs, const vec3<T>& rhs){
        return vec3<T>(
            lhs.a * rhs.x + lhs.b * rhs.y + lhs.c * rhs.z,
            lhs.d * rhs.x + lhs.e * rhs.y + lhs.f * rhs.z,
            lhs.g * rhs.x + lhs.h * rhs.y + lhs.i * rhs.z
        );
    }
    friend mat3<T> operator*(mat3<T> lhs, const mat3<T>& rhs){return lhs *= rhs;}
    friend mat3<T> operator/(mat3<T> lhs, const T& rhs){ return lhs /= rhs; }


    /* compound assignment */
    mat3<T>& operator+=(const mat3<T>& rhs){
        a+=rhs.a; b+=rhs.b; c+=rhs.c;
        d+=rhs.d; e+=rhs.e; f+=rhs.f;
        g+=rhs.g; h+=rhs.h; i+=rhs.i;
        return *this;
    }
    mat3<T>& operator-=(const mat3<T>& rhs){
        a-=rhs.a; b-=rhs.b; c-=rhs.c;
        d-=rhs.d; e-=rhs.e; f-=rhs.f;
        g-=rhs.g; h-=rhs.h; i-=rhs.i;
        return *this;
    }
    mat3<T>& operator*=(const mat3<T>& rhs){
        T _a = a, _b = b, _c = c, _d = d, _e = e, _f=f,_g=g,_h=h,_i=i;
        a = _a * rhs.a + _b * rhs.d + _c * rhs.g;
        b = _a * rhs.b + _b * rhs.e + _c * rhs.h;
        c = _a * rhs.c + _b * rhs.f + _c * rhs.i;

        d = _d * rhs.a + _e * rhs.d + _f * rhs.g;
        e = _d * rhs.b + _e * rhs.e + _f * rhs.h;
        f = _d * rhs.c + _e * rhs.f + _f * rhs.i;

        g = _g * rhs.a + _h * rhs.d + _i * rhs.g;
        h = _g * rhs.b + _h * rhs.e + _i * rhs.h;
        i = _g * rhs.c + _h * rhs.f + _i * rhs.i;
        return *this;
    }
    mat3<T>& operator*=(const T& rhs){
        a*=rhs; b*=rhs; c*=rhs;
        d*=rhs; e*=rhs; f*=rhs;
        g*=rhs; h*=rhs; i*=rhs;
        return *this;
    }
    mat3<T>& operator/=(const T& rhs){
        a/=rhs; b/=rhs; c/=rhs;
        d/=rhs; e/=rhs; f/=rhs;
        g/=rhs; h/=rhs; i/=rhs;
        return *this;
    }

    /* Member access */
    T** operator[](int index){
        return values[index];
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const mat3<U>&);
};

template<class U>
std::ostream &operator<<(std::ostream &out, const mat3<U> &mat) {
    out << "[\n\t" << mat.a << ", " << mat.b << ", " << mat.c << ";\n\t"
    << mat.d << ", " << mat.e << ", " << mat.f << ";\n\t"
    << mat.g << ", " << mat.h << ", " << mat.i << "\n]\n";
    return out;
}
