cmake_minimum_required(VERSION 3.18)
project(math-tests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(math-tests vec/TestVec2.h vec/TestVec3.h vec/TestVec4.h Test.h main.cpp mat/TestMat2.h mat/TestMat3.h mat/TestMat4.h)
