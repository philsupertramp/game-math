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

using vec2f = vec2<float>;
using vec2i = vec2<int>;
using vec2d = vec2<double>;

using vec3f = vec3<float>;
using vec3i = vec3<int>;
using vec3d = vec3<double>;

struct cmp_vec3i {
    bool operator()(const vec3i& a, const vec3i& b) const { return a.x == b.x && a.y == b.y && a.z == b.z; }
};

using vec4f = vec4<float>;
using vec4i = vec4<int>;
using vec4d = vec4<double>;

using mat2f = mat2<float>;
using mat2i = mat2<int>;
using mat2d = mat2<double>;

using mat3f = mat3<float>;
using mat3i = mat3<int>;
using mat3d = mat3<double>;

using mat4f = mat4<float>;
using mat4i = mat4<int>;
using mat4d = mat4<double>;
