/**
 * @file
 *
 * Interface for typed 2,3,4D-vectors/matrices.
 *
 * Contains aliases for typed types.
 */
#pragma once

#include "Random.h"
#include "format.h"
#include "mat/mat2.h"
#include "mat/mat3.h"
#include "mat/mat4.h"
#include "utils.h"
#include "vec/vec2.h"
#include "vec/vec3.h"
#include "vec/vec4.h"

/**
 * float vec2
 */
using vec2f = vec2<float>;
/**
 * int vec2
 */
using vec2i = vec2<int>;
/**
 * double vec2
 */
using vec2d = vec2<double>;

/**
 * float vec3
 */
using vec3f = vec3<float>;
/**
 * int vec3
 */
using vec3i = vec3<int>;
/**
 * double vec3
 */
using vec3d = vec3<double>;

/**
 * vec3 comparison for std::map
 */
struct cmp_vec3i {
  /**
   * comparison operator of two vec3i objects
   * @param a
   * @param b
   * @return
   */
  bool operator()(const vec3i& a, const vec3i& b) const { return a.x == b.x && a.y == b.y && a.z == b.z; }
};

/**
 * float vec4
 */
using vec4f = vec4<float>;
/**
 * int vec4
 */
using vec4i = vec4<int>;
/**
 * double vec4
 */
using vec4d = vec4<double>;

/**
 * float mat2
 */
using mat2f = mat2<float>;
/**
 * int mat2
 */
using mat2i = mat2<int>;
/**
 * double mat2
 */
using mat2d = mat2<double>;

/**
 * float mat3
 */
using mat3f = mat3<float>;
/**
 * int mat3
 */
using mat3i = mat3<int>;
/**
 * double mat3
 */
using mat3d = mat3<double>;

/**
 * float mat4
 */
using mat4f = mat4<float>;
/**
 * int mat4
 */
using mat4i = mat4<int>;
/**
 * double mat4
 */
using mat4d = mat4<double>;
