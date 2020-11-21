cmake_minimum_required(VERSION 3.18)
project(math-tests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(MATH_SILENCE_WARNING)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(COMPILER_CLANG)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_GCC)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_VC)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(math-tests vec/TestVec2.h vec/TestVec3.h vec/TestVec4.h Test.h main.cpp mat/TestMat2.h mat/TestMat3.h mat/TestMat4.h ode/TestExplicitEuler.h ode/TestODE45.h)
