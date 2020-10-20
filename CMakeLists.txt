cmake_minimum_required(VERSION 3.18)
project(math)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(math SHARED include/vec2.h tests/TestVec2.h tests/Test.h include/vec3.h)

add_executable(test main.cpp)
