#pragma once

#include <ostream>
#include <math.h>

template<class T>
class vec2
{
    T* values[2];
public:
    T x;
    T y;

    vec2(){
        x = static_cast<T>(0.0f);
        y = static_cast<T>(0.0f);
        values[0] = &x;
        values[1] = &y;
    }
    vec2(T _x, T _y){
        x = _x;
        y = _y;
        values[0] = &x;
        values[1] = &y;
    }

    /* Misc functions */
    inline float cross(vec2<T>& rhs){ return ((float)x*rhs.y - (float)y*rhs.x); }
    inline float length(){ return sqrtf(x*x+y*y); }
    inline vec2<T> normalize(){ return *this / length(); }

    /* Arithmetic operators */
    friend vec2<T> operator+(vec2<T> lhs, const vec2<T>& rhs){ return lhs += rhs; }
    friend vec2<T> operator-(vec2<T> lhs, const vec2<T>& rhs){ return lhs -= rhs; }
    friend vec2<T> operator*(vec2<T> lhs, const T& rhs){ return lhs *= rhs; }
    friend T operator*(vec2<T> lhs, const vec2<T>& rhs){ return lhs.x * rhs.x + lhs.y * rhs.y; }
    friend vec2<T> operator/(vec2<T> lhs, const T& rhs){ return lhs /= rhs; }

    /* compound assignment */
    vec2<T>& operator+=(const vec2<T>& rhs){
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    vec2<T>& operator-=(const vec2<T>& rhs){
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    vec2<T>& operator*=(const T& rhs){
        x *= rhs;
        y *= rhs;
        return *this;
    }
    vec2<T>& operator/=(const T& rhs){
        x /= rhs;
        y /= rhs;
        return *this;
    }

    /* comparison operators */
    /* explicit type casts */


    /* Member access */
    T* operator[](int index){
        return values[index];
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const vec2<U>&);
};

template<class T>
std::ostream &operator<<(std::ostream &os, const vec2<T> &obj) {
    os << obj.x << ", " << obj.y << std::endl;
    return os;
}


template<class T>
vec2<T> build_vec2(void* in){
    T* values = (T*)in;
    vec2<T> out(values[0], values[1]);
    return out;
}