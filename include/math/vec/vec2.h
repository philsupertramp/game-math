#pragma once

#include <cmath>
#include <ostream>

/**
 * 2D vector representation
 * @tparam T
 */
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
    //! packed data
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

    //! object dimension
    const size_t dim = 2;

    /**
     * default constructor
     */
    vec2() {
        x = static_cast<T>(0.0f);
        y = static_cast<T>(0.0f);
    }
    /**
     * two-element constructor
     * @param _x
     * @param _y
     */
    vec2(T _x, T _y) {
        x = _x;
        y = _y;
    }

    /**
     * single value constructor initializes all elements with given _v
     * @param _v
     */
    explicit vec2(T _v) {
        x = _v;
        y = _v;
    }

    /**
     * copy constructor
     * @param _v
     */
    vec2(const vec2<T>& _v) {
        x = _v.x;
        y = _v.y;
    }

    /* Misc functions */
    /**
     * length getter
     * @return
     */
    inline float length() const { return sqrtf(x * x + y * y); }
    /**
     * getter for normalized vector
     * @return
     */
    inline vec2<T> normalize() const { return vec2(x / length(), y / length()); }

    /* Arithmetic operators */
    /**
     * vec-vec addition
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec2<T> operator+(vec2<T> lhs, const vec2<T>& rhs) { return lhs += rhs; }
    /**
     * vec-vec subtraction
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec2<T> operator-(vec2<T> lhs, const vec2<T>& rhs) { return lhs -= rhs; }
    /**
     * vec-scalar multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec2<T> operator*(vec2<T> lhs, const T& rhs) { return lhs *= rhs; }
    /**
     * scalar-vector multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec2<T> operator*(const T& lhs, vec2<T> rhs) { return rhs *= lhs; }

    /**
     * vector-vector multiplication (dot-product)
     * @param lhs
     * @param rhs
     * @return
     */
    friend T operator*(vec2<T> lhs, const vec2<T>& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }

    /**
     * vector-scalar division
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec2<T> operator/(vec2<T> lhs, const T& rhs) { return lhs /= rhs; }

    /* compound assignment */
    /**
     * vector-vector addition
     * @param rhs
     * @return
     */
    vec2<T>& operator+=(const vec2<T>& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    /**
     * vector-vector subtraction
     * @param rhs
     * @return
     */
    vec2<T>& operator-=(const vec2<T>& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    /**
     * vector-scalar multiplication
     * @param rhs
     * @return
     */
    vec2<T>& operator*=(const T& rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    /**
     * vector-scalar division
     * @param rhs
     * @return
     */
    vec2<T>& operator/=(const T& rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    /* comparison operators */

    /**
     * equality comparison operator
     * @param lhs
     * @param rhs
     * @return
     */
    friend bool operator==(const vec2<T>& lhs, const vec2<T>& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    /* explicit type casts */

    /**
     * assignment operator
     * @param V
     * @return
     */
    constexpr vec2<T>& operator=(vec2<T> const& V) {
        this->x = static_cast<T>(V.x);
        this->y = static_cast<T>(V.y);
        return *this;
    }

    /* Member access */
    /**
     * member access
     * @param index
     * @return
     */
    T& operator[](int index) {
        switch(index) {
            default:
            case 0: return x;
            case 1: return y;
        }
    }
    /**
     * const member access
     * @param index
     * @return
     */
    const T& operator[](int index) const {
        switch(index) {
            default:
            case 0: return x;
            case 1: return y;
        }
    }

    /**
     * cast into other type
     * @tparam U
     * @return
     */
    template<typename U>
    operator vec2<U>() {
        return vec2<U>(x, y);
    }

    // LCOV_EXCL_START
    /* stream operators */
    /**
     * beautified std::cout operator
     * @tparam U
     * @return
     */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const vec2<U>&);
    // LCOV_EXCL_STOP
};

template<class T>
std::ostream& operator<<(std::ostream& os, const vec2<T>& obj) {
    os.precision(17);
    os << obj.x << ", " << obj.y << std::endl;
    return os;
}

/**
 * cast from void* to vec2
 * @tparam T
 * @param in
 * @return
 */
template<class T>
vec2<T> build_vec2(void* in) {
    T* values = (T*)in;
    return vec2<T>(values[0], values[1]);
}

/**
 * \example vec/TestVec2.cpp
 * This is an example on how to use the vec2.h file.
 *
 */