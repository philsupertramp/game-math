#pragma once

#include <cmath>
#include <ostream>

template<class T>
struct vec2 {
public:
#ifdef MATH_SILENCE_WARNING
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
            T x, y;
        };
    };

#ifdef MATH_SILENCE_WARNING
    #if COMPILER_CLANG
        #pragma clang diagnostic pop
    #elif COMPILER_GCC
        #pragma GCC diagnostic pop
    #elif COMPILER_VC
        #pragma warning(pop)
    #endif
#endif

    vec2() {
        x = static_cast<T>(0.0f);
        y = static_cast<T>(0.0f);
    }
    vec2(T _x, T _y) {
        x = _x;
        y = _y;
    }
    explicit vec2(T _v) {
        x = _v;
        y = _v;
    }
    constexpr vec2(const vec2<T>& _v) {
        x = _v.x;
        y = _v.y;
    }

    /* Misc functions */
    inline float length() { return sqrtf(x * x + y * y); }
    inline vec2<T> normalize() { return *this / length(); }

    /* Arithmetic operators */
    friend vec2<T> operator+(vec2<T> lhs, const vec2<T>& rhs) { return lhs += rhs; }
    friend vec2<T> operator-(vec2<T> lhs, const vec2<T>& rhs) { return lhs -= rhs; }
    friend vec2<T> operator*(vec2<T> lhs, const T& rhs) { return lhs *= rhs; }
    friend vec2<T> operator*(const T& lhs, vec2<T> rhs) { return rhs *= lhs; }
    friend T operator*(vec2<T> lhs, const vec2<T>& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
    friend vec2<T> operator/(vec2<T> lhs, const T& rhs) { return lhs /= rhs; }

    /* compound assignment */
    vec2<T>& operator+=(const vec2<T>& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    vec2<T>& operator-=(const vec2<T>& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    vec2<T>& operator*=(const T& rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    vec2<T>& operator/=(const T& rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    /* comparison operators */

    friend bool operator==(const vec2<T>& lhs, const vec2<T>& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    /* explicit type casts */

    constexpr vec2<T>& operator=(vec2<T> const& V) {
        this->x = static_cast<T>(V.x);
        this->y = static_cast<T>(V.y);
        return *this;
    }

    /* Member access */
    T& operator[](int index) {
        switch(index) {
            default:
            case 0: return x;
            case 1: return y;
        }
    }
    const T& operator[](int index) const {
        switch(index) {
            default:
            case 0: return x;
            case 1: return y;
        }
    }

    template<typename U>
    operator vec2<U>(){
        return vec2<U>(x, y);
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const vec2<U>&);
};

template<class T>
std::ostream& operator<<(std::ostream& os, const vec2<T>& obj) {
    os << obj.x << ", " << obj.y << std::endl;
    return os;
}


template<class T>
vec2<T> build_vec2(void* in) {
    T* values = (T*)in;
    return vec2<T>(values[0], values[1]);
}
