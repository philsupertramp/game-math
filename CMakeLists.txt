cmake_minimum_required(VERSION 3.18)
project(math-lib)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_SOURCES
        include/math/math.h
        include/math/vec/vec2.h
        include/math/vec/vec3.h
        include/math/vec/vec4.h
        include/math/mat/mat2.h
        include/math/mat/mat3.h
        include/math/mat/mat4.h
)

add_library(math-lib SHARED ${LIB_SOURCES})
add_subdirectory(tests)
target_include_directories(math-tests PUBLIC include)