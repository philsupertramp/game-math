#pragma once

#include <cmath>
#include <ostream>

template<class T>
struct vec2
{
public:
    union
    {
        struct
        {
            T x, y;
        };
    };

    vec2()
    {
        x = static_cast<T>(0.0f);
        y = static_cast<T>(0.0f);
    }
    vec2(T _x, T _y)
    {
        x = _x;
        y = _y;
    }
    vec2(T _x)
    {
        x = _x;
        y = _x;
    }

    /* Misc functions */
    inline float length() const { return sqrtf(x * x + y * y); }
    inline vec2<T> normalized() { return *this / length(); }

    /* Arithmetic operators */
    auto operator+(const vec2<T>& other) const { return vec2{ this->x + other.x, this->y + other.y }; }
    friend vec2 operator-(vec2 lhs, const vec2& rhs) { return lhs -= rhs; }
    friend vec2 operator*(vec2 lhs, const T& rhs) { return lhs *= rhs; }
    friend vec2 operator*(const T& lhs, vec2 rhs) { return rhs *= lhs; }
    friend T operator*(vec2 lhs, const vec2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
    friend vec2 operator/(vec2 lhs, const T& rhs) { return lhs /= rhs; }

    /* compound assignment */
    vec2<T>& operator+=(const vec2<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    vec2<T>& operator-=(const vec2<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    vec2<T>& operator*=(const T& rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    vec2<T>& operator/=(const T& rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    /* comparison operators */

    bool operator==(const vec2<T>& other) const { return this->x == other.x && this->y == other.y; }
    bool operator!=(const vec2<T>& other) const { return !operator==(other); }
    /* explicit type casts */

    vec2<T>& operator=(vec2<T> const& V)
    {
        this->x = static_cast<T>(V.x);
        this->y = static_cast<T>(V.y);
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
        }
    }
    const T& operator[](int index) const
    {
        switch(index)
        {
        default:
        case 0: return x;
        case 1: return y;
        }
    }

    /* stream operators */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const vec2<U>&);
};

template<class T>
std::ostream& operator<<(std::ostream& os, const vec2<T>& obj)
{
    return os << obj.x << ", " << obj.y << std::endl;
}


template<class T>
vec2<T> build_vec2(void* in)
{
    T* values = (T*)in;
    return vec2<T>(values[0], values[1]);
}
