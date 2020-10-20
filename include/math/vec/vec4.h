#pragma once

#include <ostream>
#include <cmath>

template<class T>
class vec4
{
    T* values[4];
public:
    T x;
    T y;
    T z;
    T w;

    vec4(){
        x = static_cast<T>(0.0f);
        y = static_cast<T>(0.0f);
        z = static_cast<T>(0.0f);
        w = static_cast<T>(0.0f);
        values[0] = &x;
        values[1] = &y;
        values[2] = &z;
        values[3] = &w;
    }
    vec4(T _x, T _y, T _z, T _w){
        x = _x; values[0] = &x;
        y = _y; values[1] = &y;
        z = _z; values[2] = &z;
        w = _w; values[3] = &w;
    }

    /* Misc functions */
    inline float length(){ return sqrtf(x*x+y*y+z*z+w*w); }
    inline vec4<T> normalize(){ return *this / length(); }

    /* Arithmetic operators */
    friend vec4<T> operator+(vec4<T> lhs, const vec4<T>& rhs){ return lhs += rhs; }
    friend vec4<T> operator-(vec4<T> lhs, const vec4<T>& rhs){ return lhs -= rhs; }
    friend vec4<T> operator*(vec4<T> lhs, const T& rhs){ return lhs *= rhs; }
    friend T operator*(vec4<T> lhs, const vec4<T>& rhs){ return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * lhs.w; }
    friend vec4<T> operator/(vec4<T> lhs, const T& rhs){ return lhs /= rhs; }

    /* compound assignment */
    vec4<T>& operator+=(const vec4<T>& rhs){
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }
    vec4<T>& operator-=(const vec4<T>& rhs){
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }
    vec4<T>& operator*=(const T& rhs){
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }
    vec4<T>& operator/=(const T& rhs){
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    /* comparison operators */
    friend bool operator==(const vec4<T>& lhs, const vec4<T>& rhs){ return lhs.x==rhs.x&&lhs.y==rhs.y&&lhs.z==rhs.z&&lhs.w==rhs.w; }
    /* explicit type casts */


    /* Member access */
    T& operator[](int index){
        return *values[index];
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const vec4<U>&);
};

template<class T>
std::ostream &operator<<(std::ostream &os, const vec4<T> &obj) {
    os << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << std::endl;
    return os;
}


template<class T>
vec4<T> build_vec4(void* in){
    T* values = (T*)in;
    vec4<T> out(values[0], values[1], values[2], values[3]);
    return out;
}