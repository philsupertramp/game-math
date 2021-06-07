#pragma once

#include <cmath>
#include <ostream>

/**
 * 4D-vector implementation
 * @tparam T
 */
template<class T>
struct vec4 {
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
    //! representation of packed data
    union {
        struct {
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

    //! dimension of object
    const size_t dim = 4;

    /**
     * empty constructor initializes object with element value 0
     */
    vec4() {
        x = static_cast<T>(0.0f);
        y = static_cast<T>(0.0f);
        z = static_cast<T>(0.0f);
        w = static_cast<T>(0.0f);
    }

    /**
     * 4-element constructor
     * @param _x
     * @param _y
     * @param _z
     * @param _w
     */
    vec4(T _x, T _y, T _z, T _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    /**
     * data array constructor
     * @param v
     */
    vec4(T* v) {
        x = v[0];
        y = v[1];
        z = v[2];
        w = v[3];
    }

    /**
     * copy constructor
     * @param v
     */
    vec4(const vec4<T>& v) {
        x = v[0];
        y = v[1];
        z = v[2];
        w = v[3];
    }

    /* Misc functions */
    /**
     * Getter for vector length
     * @return
     */
    inline float length() const { return sqrtf(x * x + y * y + z * z + w * w); }

    /**
     * Getter for normalized vector
     * @return
     */
    inline vec4<T> normalize() const { return *this / length(); }

    /* Arithmetic operators */
    /**
     * vector-vector addition
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec4<T> operator+(vec4<T> lhs, const vec4<T>& rhs) { return lhs += rhs; }
    /**
     * vector-vector subtraction
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec4<T> operator-(vec4<T> lhs, const vec4<T>& rhs) { return lhs -= rhs; }

    /**
     * vector-constant multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec4<T> operator*(vec4<T> lhs, const T& rhs) { return lhs *= rhs; }

    /**
     * scalar-vector multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec4<T> operator*(const T& lhs, vec4<T> rhs) { return rhs *= lhs; }

    /**
     * Dot product implementation
     * @param lhs
     * @param rhs
     * @return
     */
    friend T operator*(vec4<T> lhs, const vec4<T>& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * lhs.w;
    }
    /**
     * vector-scalar division
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec4<T> operator/(vec4<T> lhs, const T& rhs) { return lhs /= rhs; }

    /* compound assignment */
    /**
     * vector-vector addition
     * @param rhs
     * @return
     */
    vec4<T>& operator+=(const vec4<T>& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }
    /**
     * vector-vector subtraction
     * @param rhs
     * @return
     */
    vec4<T>& operator-=(const vec4<T>& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    /**
     * vector-scalar multiplication
     * @param rhs
     * @return
     */
    vec4<T>& operator*=(const T& rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    /**
     * vector-scalar division
     * @param rhs
     * @return
     */
    vec4<T>& operator/=(const T& rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    /* comparison operators */
    /**
     * equality comparison
     * @param lhs
     * @param rhs
     * @return
     */
    friend bool operator==(const vec4<T>& lhs, const vec4<T>& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
    }
    /* explicit type casts */

    /**
     * const copy constructor
     * @param V
     * @return
     */
    constexpr vec4<T>& operator=(vec4<T> const& V) {
        this->x = static_cast<T>(V.x);
        this->y = static_cast<T>(V.y);
        this->z = static_cast<T>(V.z);
        this->w = static_cast<T>(V.w);
        return *this;
    }

    /* Member access */
    /**
     * element access
     * @param index
     * @return
     */
    T& operator[](int index) {
        switch(index) {
            default:
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
        }
    }
    /**
     * const element access
     * @param index
     * @return
     */
    const T& operator[](int index) const {
        switch(index) {
            default:
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
        }
    }

    /**
     * cast operator into new type
     *
     * @tparam U
     * @return vec4<U>(vec4<T>)
     */
    template<typename U>
    operator vec4<U>() {
        return vec4<U>(x, y, z, w);
    }

    // LCOV_EXCL_START
    /* stream operators */
    /**
     * beautified std::cout operator
     * @tparam U
     * @return
     */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const vec4<U>&);
    // LCOV_EXCL_STOP
};

template<class T>
std::ostream& operator<<(std::ostream& os, const vec4<T>& obj) {
    os.precision(17);
    os << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << std::endl;
    return os;
}

/**
 * cast from 4d-array of values to vec4
 * @tparam T
 * @param in
 * @return
 */
template<class T>
vec4<T> build_vec4(void* in) {
    T* values = (T*)in;
    vec4<T> out(values[0], values[1], values[2], values[3]);
    return out;
}

/**
 * \example vec/TestVec4.cpp
 * This is an example on how to use the vec4.h file.
 *
 */