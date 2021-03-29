cmake_minimum_required(VERSION 3.18)
project(math-tests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#
#OPTION(ENABLE_GCOV "Enable gcov (debug, Linux builds only)" OFF)
IF (MATH_COVERAGE)
    string(APPEND CMAKE_C_FLAGS " -fprofile-arcs -ftest-coverage -DHAVE_GCOV")
    string(APPEND CMAKE_CXX_FLAGS " -fprofile-arcs -ftest-coverage -DHAVE_GCOV")
    string(APPEND CMAKE_EXE_LINKER_FLAGS " -fprofile-arcs -ftest-coverage -lgcov")
ENDIF()

set(TEST_SOURCES

        vec/TestVec2.cpp
        vec/TestVec3.cpp
        vec/TestVec4.cpp

        mat/TestMat2.cpp
        mat/TestMat3.cpp
        mat/TestMat4.cpp

        numerics/TestUtils.cpp
        numerics/ode/TestODE45.cpp
        numerics/ode/TestExplicitEuler.cpp

        numerics/lin_alg/TestBackwardSub.cpp
        numerics/lin_alg/TestForwardSub.cpp
        numerics/lin_alg/TestGaussianElimination.cpp
        numerics/lin_alg/TestLU.cpp

        ds/TestDSUtils.cpp
        ds/TestMatrixDS.cpp
        ds/TestPerceptron.cpp
        ds/TestAdalineGD.cpp
        ds/TestAdalineSGD.cpp

        )

find_program(CTEST_COVERAGE_COMMAND NAMES gcov)
include(CTest)
enable_testing()

include_directories(../include)

function(add_test_source SOURCE_NAME)
    string(REPLACE ".cpp" "" NAME ${SOURCE_NAME})
    string(REPLACE "/" "_" NAME ${NAME})
    message("building ${NAME} ${SOURCE_NAME}")
    add_executable(${NAME} ${SOURCE_NAME})
    target_link_libraries(${NAME} math-lib)
    add_test(${NAME} ${NAME})
endfunction()


add_test_source(vec/TestVec2.cpp)
add_test_source(vec/TestVec3.cpp)
add_test_source(vec/TestVec4.cpp)

add_test_source(mat/TestMat2.cpp)
add_test_source(mat/TestMat3.cpp)
add_test_source(mat/TestMat4.cpp)

add_test_source(numerics/TestUtils.cpp)
add_test_source(numerics/ode/TestODE45.cpp)
add_test_source(numerics/ode/TestExplicitEuler.cpp)

add_test_source(numerics/lin_alg/TestBackwardSub.cpp)
add_test_source(numerics/lin_alg/TestForwardSub.cpp)
add_test_source(numerics/lin_alg/TestGaussianElimination.cpp)
add_test_source(numerics/lin_alg/TestLU.cpp)

add_test_source(ds/TestDSUtils.cpp)
add_test_source(ds/TestMatrixDS.cpp)
add_test_source(ds/TestPerceptron.cpp)
add_test_source(ds/TestAdalineGD.cpp)
add_test_source(ds/TestAdalineSGD.cpp)

