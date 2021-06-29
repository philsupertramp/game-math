#pragma once

#include "vec4.h"
#include <cmath>
#include <ostream>

/**
 * 3D vector representation
 * @tparam T
 */
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

    //! packed data representation
    union {
        struct {
            T x, y, z;
        };
    };

    //! dimension of object
    const size_t dim = 3;

#if MATH_SILENCE_WARNING
    #if COMPILER_CLANG
        #pragma clang diagnostic pop
    #elif COMPILER_GCC
        #pragma GCC diagnostic pop
    #elif COMPILER_VC
        #pragma warning(pop)
    #endif
#endif

    /**
     * default constructor initializes elements with 0
     */
    vec3() {
        x = static_cast<T>(0.0f);
        y = static_cast<T>(0.0f);
        z = static_cast<T>(0.0f);
    }

    /**
     * 3 element
     * @param _x
     * @param _y
     * @param _z
     */
    vec3(const T& _x, const T& _y, const T& _z) {
        this->x = _x;
        this->y = _y;
        this->z = _z;
    }

    /**
     * vec4 to vec3 cast
     * @param a
     */
    vec3(const vec4<T>& a) {
        this->x = a.x;
        this->y = a.y;
        this->z = a.z;
    }

    /**
     * vec2 + T constructor
     * @param a 2D vector
     * @param z element z
     */
    vec3(const vec2<T>& a, const T& z) {
        this->x = a.x;
        this->y = a.y;
        this->z = z;
    }

    /**
     * single value constructor
     * @param _v
     */
    vec3(const T& _v) {
        this->x = _v;
        this->y = _v;
        this->z = _v;
    }
    /**
     * copy-constructor
     * @param v
     */
    vec3(const vec3<T>& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }


    /* Misc functions */
    /**
     * cross-product
     * @param rhs
     * @return
     */
    inline vec3<T> cross(const vec3<T>& rhs) const {
        return vec3<T>(
        (float)y * rhs.z - (float)z * rhs.y, (float)z * rhs.x - (float)x * rhs.z, (float)x * rhs.y - y * rhs.x);
    }
    /**
     * calculates the vectors length (p=2-norm)
     *
     * \f[
     *  \sqrt{\sum x_i}
     * \f]
     * @return
     */
    inline float length() const { return sqrtf(x * x + y * y + z * z); }
    /**
     * returns normalized vector
     * @return
     */
    inline vec3<T> normalize() const {
        float len = length();
        return ((len != 0.0f) ? (*this / len) : *this);
    }

    /* Arithmetic operators */
    /**
     * vector-vector addition
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec3<T> operator+(vec3<T> lhs, const vec3<T>& rhs) { return lhs += rhs; }
    /**
     * vector-vector subtraction
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec3<T> operator-(vec3<T> lhs, const vec3<T>& rhs) { return lhs -= rhs; }

    /**
     * vector-scalar multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec3<T> operator*(vec3<T> lhs, const T& rhs) { return lhs *= rhs; }

    /**
     * vector-scalar multiplication
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec3<T> operator*(const T& lhs, vec3<T> rhs) { return rhs *= lhs; }

    /**
     * vector-vector multiplication aka dot-product
     * @param lhs
     * @param rhs
     * @return
     */
    friend T operator*(vec3<T> lhs, const vec3<T>& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }

    /**
     * vector-scalar division
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec3<T> operator/(vec3<T> lhs, const T& rhs) { return lhs /= rhs; }

    // element wise division
    /**
     * vector-scalar division
     * @param lhs
     * @param rhs
     * @return
     */
    friend vec3<T> operator/(vec3<T> lhs, const vec3<T>& rhs) {
        return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
    }

    /* compound assignment */
    /**
     * vector-vector addition
     * @param rhs
     * @return
     */
    vec3<T>& operator+=(const vec3<T>& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }
    /**
     * vector-vector subtraction
     * @param rhs
     * @return
     */
    vec3<T>& operator-=(const vec3<T>& rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }
    /**
     * vector-scalar multiplication
     * @param rhs
     * @return
     */
    vec3<T>& operator*=(const T& rhs) {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }
    /**
     * vector-scalar division
     * @param rhs
     * @return
     */
    vec3<T>& operator/=(const T& rhs) {
        this->x /= rhs;
        this->y /= rhs;
        this->z /= rhs;
        return *this;
    }

    /* comparison operators */
    /**
     * equality comparison
     * @param lhs
     * @param rhs
     * @return
     */
    friend bool operator==(const vec3<T>& lhs, const vec3<T>& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }

    /**
     * bigger comparison operator
     * @param lhs
     * @param rhs
     * @return
     */
    friend bool operator>(const vec3<T>& lhs, const vec3<T>& rhs) { return lhs.length() > rhs.length(); }

    /**
     * smaller comparison operator
     * @param lhs
     * @param rhs
     * @return
     */
    friend bool operator<(const vec3<T>& lhs, const vec3<T>& rhs) { return lhs.length() < rhs.length(); }

    /* explicit type casts */

    /**
     * copy constructor
     * @param V
     * @return
     */
    constexpr vec3<T>& operator=(vec3<T> const& V) {
        this->x = static_cast<T>(V.x);
        this->y = static_cast<T>(V.y);
        this->z = static_cast<T>(V.z);
        return *this;
    }

    /* Member access */
    /**
     * element access
     * @param index
     * @return
     */
    T& operator[](const int& index) {
        switch(index) {
            default:
            case 0: return x;
            case 1: return y;
            case 2: return z;
        }
    }
    /**
     * const element access
     * @param index
     * @return
     */
    const T& operator[](const int& index) const {
        switch(index) {
            default:
            case 0: return x;
            case 1: return y;
            case 2: return z;
        }
    }

    /**
     * cast operator into new type U
     * @tparam U
     * @return vec3<U>(vec3<T>)
     */
    template<typename U>
    explicit operator vec3<U>() {
        return vec3<U>(x, y, z);
    }

    /* stream operators */
    /**
     * beautified std::cout operator
     * @tparam U
     * @return
     */
    template<class U>
    friend std::ostream& operator<<(std::ostream&, const vec3<U>&);
};

template<class T>
std::ostream& operator<<(std::ostream& os, const vec3<T>& obj) {
    os.precision(17);

    os << obj.x << ", " << obj.y << ", " << obj.z << std::endl;
    return os;
}


/**
 * cast from 3d-array to vec3
 * @tparam T
 * @param in
 * @return
 */
template<class T>
vec3<T> build_vec3(void* in) {
    T* values = (T*)in;
    vec3<T> out(values[0], values[1], values[2]);
    return out;
}

namespace std {
    /**
     * vec3 implementation of numeric_limits
     * @tparam T
     */
    template<typename T>
    class numeric_limits<vec3<T>>
    {
    public:
        /**
         * static getter for lowes value
         * @return
         */
        static vec3<T> lowest() { return vec3<T>(std::numeric_limits<T>::lowest()); };
        /**
         * static getter for maximal value
         * @return
         */
        static vec3<T> max() { return vec3<T>(std::numeric_limits<T>::max()); };
        /**
         * static getter for minimal value
         * @return
         */
        static vec3<T> min() { return vec3<T>(std::numeric_limits<T>::min()); };
        // One can implement other methods if needed
    };
} // namespace std

/**
 * \example vec/TestVec3.cpp
 * This is an example on how to use the vec3.h file.
 *
 */