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
        vec3<T> zaxis = normalize(eye-center); vec3<T> xaxis = normalize(up.cross(zaxis)); vec3<T> yaxis = zaxis.cross(xaxis);
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
        mat[0][0] =(T) 2.0f / (right - left);
        mat[1][1] =(T) 2.0f / (top - bottom);
        mat[2][2] =(T) -2.0f / (zFar-zNear);
        mat[3][0] =(T) (right+left) / (left-right);
        mat[3][1] =(T) (top+bottom) / (bottom - top);
        mat[3][2] =(T) (zFar + zNear) / (zNear - zFar);
        mat[3][3] =(T) 1.0f;
        return mat;
    }

    template<class T>
    mat4<T> perspective(float FOV, float width, float height, float zNear, float zFar){
        float f = 1.0f / tanf(FOV / 2.0f);
        float nf = 1.0f / (zNear - zFar);
        float aspect = width / height;
        return mat4<T>(f / aspect, 0, 0, 0, 0, f, 0, 0, 0, 0, (zFar + zNear)*nf, -1, 0, 0, (2*zFar*zNear) * nf, 0);
    }

    template<class T>
    mat4<T> angleAxis(float angle, vec3<T> axis){
        vec4<T> q(axis.x, axis.y, axis.z, angle);
    }

    template<class T>
    mat4<T> scale(mat4<T> mat, float factor){
        return mat4<T>::Unit() * factor * mat;
    }

    template<class T>
    mat4<T> rotate(mat4<T> mat, float rad, vec3<T> axis){

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