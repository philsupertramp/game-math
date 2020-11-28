cmake_minimum_required(VERSION 3.18)
project(math-tests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(math-tests vec/TestVec2.h vec/TestVec3.h vec/TestVec4.h Test.h main.cpp mat/TestMat2.h mat/TestMat3.h mat/TestMat4.h numerics/ode/TestExplicitEuler.h numerics/ode/TestODE45.h)
