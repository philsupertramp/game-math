#pragma once
#include "vec/vec2.h"
#include "vec/vec3.h"
#include "vec/vec4.h"
#include "mat/mat2.h"
#include "mat/mat3.h"
#include "mat/mat4.h"

namespace Math::Utils {

    template<class V>
    float distance(V a, V b);

    template<class T>
    vec2<T> normalize(vec2<T> a){ return a.normalize(); }

    template<class T>
    vec3<T> normalize(vec3<T> a){ return a.normalize(); }

    template<class T>
    vec4<T> normalize(vec4<T> a){ return a.normalize(); }

    template<class T>
    mat4<T> translate(mat4<T> M, vec3<T> V){
        return M + mat4<T>::Unit() * V;
    }

    template<class T>
    mat4<T> lookAt(vec3<T> eye, vec3<T> center, vec3<T> up){
        vec3<T> zaxis = normalize(center - eye); vec3<T> xaxis = normalize(up.cross(zaxis)); vec3<T> yaxis = zaxis.cross(xaxis);
        return mat4<T>(
            xaxis.x, yaxis.x, zaxis.x, 0.0f,
            xaxis.y, yaxis.y, zaxis.y, 0.0f,
            xaxis.z, yaxis.z, zaxis.z, 0.0f,
            -(xaxis * eye), -(yaxis * eye), -(zaxis * eye), 1.0f
        );
    }

    template<class T>
    mat4<T> ortho(float left, float right , float bottom, float top , float zNear , float zFar){
        mat4<T> mat;
        mat.a =(T) 2.0f / (right - left);
        mat.f =(T) 2.0f / (top - bottom);
        mat.k =(T) -2.0f / (zFar-zNear);
        mat.m =(T) (right+left) / (left-right);
        mat.n =(T) (top+bottom) / (bottom - top);
        mat.o =(T) (zFar + zNear) / (zNear - zFar);
        mat.p =(T) 1.0f;
        return mat;
    }

    template<class T>
    mat4<T> perspective(float FOV, float width, float height, float zNear, float zFar){
        float f = 1.0f / tanf(FOV / 2.0f);
        float nf = 1.0f / (zNear - zFar);
        float aspect = width / height;
        return mat4<T>(f / aspect, 0, 0, 0, 0, f, 0, 0, 0, 0, (zFar + zNear)*nf, -1, 0, 0, (2*zFar*zNear) * nf, 0);
    }

}