#pragma once

#include <ostream>
#include <cmath>

template<class T>
class vec3
{
    T* values[3];
public:
    T x;
    T y;
    T z;

    vec3(){
        x = static_cast<T>(0.0f);
        y = static_cast<T>(0.0f);
        z = static_cast<T>(0.0f);
        values[0] = &x;
        values[1] = &y;
        values[2] = &z;
    }
    vec3(T _x, T _y, T _z){
        x = _x;
        y = _y;
        z = _z;
        values[0] = &x;
        values[1] = &y;
        values[2] = &z;
    }

    /* Misc functions */
    inline vec3<T> cross(vec3<T>& rhs){ return vec3<T>((float)y*rhs.z - (float)z*rhs.y, (float)z*rhs.x - (float)x*rhs.z, (float)x*rhs.y-y*rhs.x); }
    inline float length(){ return sqrtf(x*x+y*y+z*z); }
    inline vec3<T> normalize(){ return *this / length(); }

    /* Arithmetic operators */
    friend vec3<T> operator+(vec3<T> lhs, const vec3<T>& rhs){ return lhs += rhs; }
    friend vec3<T> operator-(vec3<T> lhs, const vec3<T>& rhs){ return lhs -= rhs; }
    friend vec3<T> operator*(vec3<T> lhs, const T& rhs){ return lhs *= rhs; }
    friend T operator*(vec3<T> lhs, const vec3<T>& rhs){ return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
    friend vec3<T> operator/(vec3<T> lhs, const T& rhs){ return lhs /= rhs; }

    /* compound assignment */
    vec3<T>& operator+=(const vec3<T>& rhs){
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    vec3<T>& operator-=(const vec3<T>& rhs){
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    vec3<T>& operator*=(const T& rhs){
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
    vec3<T>& operator/=(const T& rhs){
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    /* comparison operators */
    friend bool operator==(const vec3<T>& lhs, const vec3<T>& rhs){ return lhs.x==rhs.x&&lhs.y==rhs.y&&lhs.z==rhs.z; }
    /* explicit type casts */


    /* Member access */
    T& operator[](int index){
        return *values[index];
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const vec3<U>&);
};

template<class T>
std::ostream &operator<<(std::ostream &os, const vec3<T> &obj) {
    os << obj.x << ", " << obj.y << ", " << obj.z << std::endl;
    return os;
}


template<class T>
vec3<T> build_vec3(void* in){
    T* values = (T*)in;
    vec3<T> out(values[0], values[1], values[2]);
    return out;
}