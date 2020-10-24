#pragma once
#include "vec/vec2.h"
#include "vec/vec3.h"
#include "vec/vec4.h"
#include "mat/mat2.h"
#include "mat/mat3.h"
#include "mat/mat4.h"

namespace Math::Utils {

    template<class V>
    float distance(V a, V b){
        return (a - b).length();
    }

    template<class T>
    vec2<T> normalize(vec2<T> a){ return a.normalize(); }

    template<class T>
    vec3<T> normalize(vec3<T> a){ return a.normalize(); }

    template<class T>
    vec4<T> normalize(vec4<T> a){ return a.normalize(); }

    template<class T>
    mat4<T> translate(mat4<T> M, vec3<T> V){
        mat4<T> translation = mat4<T>::Unit();
        translation[0][0] = V.x; translation[1][1] = V.y; translation[2][2] = V.z;
        return M + translation;
    }

    template<class T>
    mat4<T> lookAt(vec3<T> eye, vec3<T> center, vec3<T> up){
        vec3<T> zaxis = normalize(center-eye); vec3<T> xaxis = normalize(zaxis.cross(up)); vec3<T> yaxis = xaxis.cross(zaxis);
        return mat4<T>(
            xaxis.x, yaxis.x, -zaxis.x, 0.0f,
            xaxis.y, yaxis.y, -zaxis.y, 0.0f,
            xaxis.z, yaxis.z, -zaxis.z, 0.0f,
            -(xaxis * eye), -(yaxis * eye), -(zaxis * eye), 1.0f
        );
    }

    template<class T>
    mat4<T> ortho(float left, float right , float bottom, float top){
        mat4<T> mat;
        mat[0][0] =(T) static_cast<T>(2) / (right - left);
        mat[1][1] =(T) static_cast<T>(2) / (top - bottom);
        mat[2][2] =(T) -static_cast<T>(1);
        mat[3][0] =(T) -(right+left) / (left-right);
        mat[3][1] =(T) -(top+bottom) / (bottom - top);
        mat[3][3] =(T) 1.0f;
        return mat;
    }

    template<class T>
    mat4<T> perspective(float FOV, float width, float height, float zNear, float zFar){
        float halfTanFOV = 1.0f / tanf(FOV / 2.0f);
        float aspect = width / height;
        return mat4<T>(static_cast<T>(1) / (aspect* halfTanFOV), 0, 0, 0,
                       0, static_cast<T>(1) / (halfTanFOV), 0, 0,
                       0, 0, (zFar/(zFar-zNear)), static_cast<T>(1),
                       0, 0, -(zFar * zNear)/(zFar - zNear), 0);
    }

    template<class T>
    mat4<T> angleAxis(float angle, vec3<T> axis){
        vec4<T> q(axis.x, axis.y, axis.z, angle);
    }

    template<class T>
    mat4<T> scale(mat4<T> mat, float factor){
        return (mat4<T>::Unit() * factor) * mat;
    }

    template<class T>
    mat4<T> rotate(mat4<T> m, float angle, vec3<T> u){
        T const cosAngle = cosf(angle);
        T const sinAngle = sinf(angle);

        vec3<T> axis = u.normalize();
        vec3<T> temp((T(1) - cosAngle) * axis);

        mat4<T> rot;
        rot[0][0] = cosAngle + temp[0] * axis[0];
        rot[0][1] = temp[0] * axis[1] + sinAngle * axis[2];
        rot[0][2] = temp[0] * axis[2] - sinAngle * axis[1];

        rot[1][0] = temp[1] * axis[0] - sinAngle * axis[2];
        rot[1][1] = cosAngle + temp[1] * axis[1];
        rot[1][2] = temp[1] * axis[2] + sinAngle * axis[0];

        rot[2][0] = temp[2] * axis[0] + sinAngle * axis[1];
        rot[2][1] = temp[2] * axis[1] - sinAngle * axis[0];
        rot[2][2] = cosAngle + temp[2] * axis[2];

        mat4<T> out;
        out[0] = m[0] * rot[0][0] + m[1] * rot[0][1] + m[2] * rot[0][2];
        out[1] = m[0] * rot[1][0] + m[1] * rot[1][1] + m[2] * rot[1][2];
        out[2] = m[0] * rot[2][0] + m[1] * rot[2][1] + m[2] * rot[2][2];
        out[3] = m[3];
        return out;
    }

    // Possibly wrong
    template<class T>
    vec3<T> max(vec3<T> a, vec3<T> b){
        return a.length() > b.length() ? a : b;
    }

    template<class T>
    vec3<T> lerp(vec3<T> p1, vec3<T> p2, float v){
        T ax = p1[0]; T ay = p1[1]; T az = p1[2];
        return vec3<T>(ax + v * (p2[0] - ax), ay + v * (p2[0] - ay), az + v * (p2[0] - az));
    }
}

template<class T>
T* value_ptr(vec2<T> &vec){ return &(vec.x);}
template<class T>
T* value_ptr(vec3<T> &vec){ return &(vec.x);}
template<class T>
T* value_ptr(vec4<T> &vec){ return &(vec.x);}
template<class T>
T* value_ptr(mat2<T> &mat){ return &(mat[0].x);}
template<class T>
T* value_ptr(mat3<T> &mat){ return &(mat[0].x);}
template<class T>
T* value_ptr(mat4<T> &mat){ return &(mat[0].x);}

template<class T>
T const* value_ptr(vec2<T> const& vec){ return &(vec.x);}
template<class T>
T const* value_ptr(vec3<T> const& vec){ return &(vec.x);}
template<class T>
T const* value_ptr(vec4<T> const& vec){ return &(vec.x);}
template<class T>
T const* value_ptr(mat2<T> const& mat){ return &(mat[0].x);}
template<class T>
T const* value_ptr(mat3<T> const& mat){ return &(mat[0].x);}
template<class T>
T const* value_ptr(mat4<T> const& mat){ return &(mat[0].x);}