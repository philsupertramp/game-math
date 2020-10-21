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
    float normalize(vec2<T> a){ return a.normalize(); }

    template<class T>
    float normalize(vec3<T> a){ return a.normalize(); }

    template<class T>
    float normalize(vec4<T> a){ return a.normalize(); }

    template<class T>
    mat4<T> translate(mat4<T> M, vec3<T> V){
        return M + mat4<T>::Unit() * V;
    }

    template<class T>
    mat4<T> lookAt(vec3<T> eye, vec3<T> center, vec3<T> up){
        vec3<T> zaxis = normalize(center - eye); vec3<T> xaxis = normalize(up.cross(zaxis)); vec3<T> yaxis = zaxis.cross(xaxis);
        mat4<T> out = mat4<T>::Transformation(mat3<T>(xaxis, yaxis, zaxis));
        mat4<T> translation = mat4<T>::Unit();
        translation[2][0] = -1 * eye.x; translation[2][1] = -1 * eye.y; translation[2][2] = -1 * eye.z;
        return translation * out;
    }

    template<class T>
    mat4<T> ortho(float left, float right , float bottom, float top , float zNear , float zFar){

    }

    template<class T>
    mat4<T> perspective(float FOV, float width, float height, float zNear, float zFar){

    }

}