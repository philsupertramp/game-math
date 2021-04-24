#pragma once

#include "vec4.h"
#include <cmath>
#include <ostream>

template<class T>
struct vec3 {
public:
#if MATH_SILENCE_WARNING
    #if COMPILER_GCC
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wpedantic"
    #elif COMPILER_CLANG
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
        #pragma clang diagnostic ignored "-Wnested-anon-types"
    #elif COMPILER_VC
        #pragma warning(push)
        #pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
    #endif
#endif
    union {
        struct {
            T x, y, z;
        };
    };

    size_t dim = 3;

#if MATH_SILENCE_WARNING
    #if COMPILER_CLANG
        #pragma clang diagnostic pop
    #elif COMPILER_GCC
        #pragma GCC diagnostic pop
    #elif COMPILER_VC
        #pragma warning(pop)
    #endif
#endif

    vec3() {
        x = static_cast<T>(0.0f);
        y = static_cast<T>(0.0f);
        z = static_cast<T>(0.0f);
    }
    vec3(T _x, T _y, T _z) {
        this->x = _x;
        this->y = _y;
        this->z = _z;
    }
    vec3(vec4<T> a) {
        this->x = a.x;
        this->y = a.y;
        this->z = a.z;
    }
    vec3(T _v) {
        this->x = _v;
        this->y = _v;
        this->z = _v;
    }
    vec3(const vec3<T>& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }


    /* Misc functions */
    inline vec3<T> cross(vec3<T> rhs) const {
        return vec3<T>(
        (float)y * rhs.z - (float)z * rhs.y, (float)z * rhs.x - (float)x * rhs.z, (float)x * rhs.y - y * rhs.x);
    }
    inline float length() const { return sqrtf(x * x + y * y + z * z); }
    inline vec3<T> normalize() const {
        float len = length();
        return ((len != 0.0f) ? (*this / len) : *this);
    }

    /* Arithmetic operators */
    friend vec3<T> operator+(vec3<T> lhs, const vec3<T>& rhs) { return lhs += rhs; }
    friend vec3<T> operator-(vec3<T> lhs, const vec3<T>& rhs) { return lhs -= rhs; }
    friend vec3<T> operator*(vec3<T> lhs, const T& rhs) { return lhs *= rhs; }
    friend vec3<T> operator*(const T& lhs, vec3<T> rhs) { return rhs *= lhs; }
    friend T operator*(vec3<T> lhs, const vec3<T>& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
    friend vec3<T> operator/(vec3<T> lhs, const T& rhs) { return lhs /= rhs; }

    // element wise division
    friend vec3<T> operator/(vec3<T> lhs, const vec3<T>& rhs) {
        return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
    }

    /* compound assignment */
    vec3<T>& operator+=(const vec3<T>& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }
    vec3<T>& operator-=(const vec3<T>& rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }
    vec3<T>& operator*=(const T& rhs) {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }
    vec3<T>& operator/=(const T& rhs) {
        this->x /= rhs;
        this->y /= rhs;
        this->z /= rhs;
        return *this;
    }

    /* comparison operators */
    friend bool operator==(const vec3<T>& lhs, const vec3<T>& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
    friend bool operator>(const vec3<T>& lhs, const vec3<T>& rhs) { return lhs.length() > rhs.length(); }
    friend bool operator<(const vec3<T>& lhs, const vec3<T>& rhs) { return lhs.length() < rhs.length(); }
    /* explicit type casts */


    constexpr vec3<T>& operator=(vec3<T> const& V) {
        this->x = static_cast<T>(V.x);
        this->y = static_cast<T>(V.y);
        this->z = static_cast<T>(V.z);
        return *this;
    }

    /* Member access */
    T& operator[](int index) {
        switch(index) {
            default:
            case 0: return x;
            case 1: return y;
            case 2: return z;
        }
    }
    const T& operator[](int index) const {
        switch(index) {
            default:
            case 0: return x;
            case 1: return y;
            case 2: return z;
        }
    }

    template<typename U>
    operator vec3<U>() {
        return vec3<U>(x, y, z);
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const vec3<U>&);
};

template<class T>
std::ostream& operator<<(std::ostream& os, const vec3<T>& obj) {
    os.precision(17);

    os << obj.x << ", " << obj.y << ", " << obj.z << std::endl;
    return os;
}


template<class T>
vec3<T> build_vec3(void* in) {
    T* values = (T*)in;
    vec3<T> out(values[0], values[1], values[2]);
    return out;
}
namespace std {
    template<typename T>
    class numeric_limits<vec3<T>>
    {
    public:
        static vec3<T> lowest() { return vec3<T>(std::numeric_limits<T>::lowest()); };
        static vec3<T> max() { return vec3<T>(std::numeric_limits<T>::max()); };
        static vec3<T> min() { return vec3<T>(std::numeric_limits<T>::min()); };
        // One can implement other methods if needed
    };
} // namespace std
