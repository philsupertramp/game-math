cmake_minimum_required(VERSION 3.18)
project(math-lib CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-DMATH_SILENCE_WARNING)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-DCOMPILER_CLANG)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_definitions(-DCOMPILER_GCC)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_definitions(-DCOMPILER_VC)
endif()

set(LIB_SOURCES
        include/math/math.h
        include/math/vec/vec2.h
        include/math/vec/vec3.h
        include/math/vec/vec4.h
        include/math/mat/mat2.h
        include/math/mat/mat3.h
        include/math/mat/mat4.h
        include/math/utils.h
        include/math/ode/ExplicitEuler.h
        include/math/ode.h
        include/math/ode/ode45.h
)

add_library(math-lib SHARED ${LIB_SOURCES})
set_target_properties(math-lib PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)
target_include_directories(math-tests PUBLIC include)