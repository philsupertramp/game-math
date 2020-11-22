#pragma once

#include <cmath>
#include <ostream>

template<class T>
struct vec4
{
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
    union
    {
        struct
        {
            T x, y, z, w;
        };
    };

#if MATH_SILENCE_WARNING
    #if COMPILER_CLANG
        #pragma clang diagnostic pop
    #elif COMPILER_GCC
        #pragma GCC diagnostic pop
    #elif COMPILER_VC
        #pragma warning(pop)
    #endif
#endif

    vec4()
    {
        x = static_cast<T>(0.0f);
        y = static_cast<T>(0.0f);
        z = static_cast<T>(0.0f);
        w = static_cast<T>(0.0f);
    }
    vec4(T _x, T _y, T _z, T _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
    vec4(T* v)
    {
        x = v[0];
        y = v[1];
        z = v[2];
        w = v[3];
    }
    constexpr vec4(const vec4<T>& v)
    {
        x = v[0];
        y = v[1];
        z = v[2];
        w = v[3];
    }

    /* Misc functions */
    inline float length() { return sqrtf(x * x + y * y + z * z + w * w); }
    inline vec4<T> normalize() { return *this / length(); }

    /* Arithmetic operators */
    friend vec4<T> operator+(vec4<T> lhs, const vec4<T>& rhs) { return lhs += rhs; }
    friend vec4<T> operator-(vec4<T> lhs, const vec4<T>& rhs) { return lhs -= rhs; }
    friend vec4<T> operator*(vec4<T> lhs, const T& rhs) { return lhs *= rhs; }
    friend vec4<T> operator*(const T& lhs, vec4<T> rhs) { return rhs *= lhs; }
    friend T operator*(vec4<T> lhs, const vec4<T>& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * lhs.w;
    }
    friend vec4<T> operator/(vec4<T> lhs, const T& rhs) { return lhs /= rhs; }

    /* compound assignment */
    vec4<T>& operator+=(const vec4<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }
    vec4<T>& operator-=(const vec4<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }
    vec4<T>& operator*=(const T& rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }
    vec4<T>& operator/=(const T& rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    /* comparison operators */
    friend bool operator==(const vec4<T>& lhs, const vec4<T>& rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
    }
    /* explicit type casts */

    constexpr vec4<T>& operator=(vec4<T> const& V)
    {
        this->x = static_cast<T>(V.x);
        this->y = static_cast<T>(V.y);
        this->z = static_cast<T>(V.z);
        this->w = static_cast<T>(V.w);
        return *this;
    }

    /* Member access */
    T& operator[](int index)
    {
        switch(index)
        {
            default:
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
        }
    }
    const T& operator[](int index) const
    {
        switch(index)
        {
            default:
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
        }
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const vec4<U>&);
};

template<class T>
std::ostream& operator<<(std::ostream& os, const vec4<T>& obj)
{
    os << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << std::endl;
    return os;
}


template<class T>
vec4<T> build_vec4(void* in)
{
    T* values = (T*)in;
    vec4<T> out(values[0], values[1], values[2], values[3]);
    return out;
}
