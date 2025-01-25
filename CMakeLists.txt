cmake_minimum_required(VERSION 3.9)
project(math-lib CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-DMATH_SILENCE_WARNING)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-DCOMPILER_CLANG)
    message("CLANG")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_definitions(-DCOMPILER_GCC)
#    set(CMAKE_CXX_FLAGS -fprofile-arcs -ftest-coverage)
    set(CMAKE_LFLAGS --coverage)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_definitions(-DCOMPILER_VC)
endif()

set(LIB_HEADERS
        include/math/math.h
        include/math/vec/vec2.h
        include/math/vec/vec3.h
        include/math/vec/vec4.h
        include/math/mat/mat2.h
        include/math/mat/mat3.h
        include/math/mat/mat4.h
        include/math/utils.h
        include/math/Random.h
        include/math/format.h
        include/math/sorting.h)

set(LIB_SOURCES

        src/format.cpp
)

add_library(math-lib SHARED ${LIB_HEADERS} ${LIB_SOURCES})
set_target_properties(math-lib PROPERTIES LINKER_LANGUAGE CXX)


if (MATH_EXTENSIONS MATCHES "(ds)")
    target_link_libraries(math-lib ${ImageMagick_LIBRARIES})
endif()

# add_compile_options(-fsanitize=address -fsanitize=undefined)
# add_link_options(-fsanitize=address -fsanitize=undefined)

if(MATH_TESTS)
    include(CTest)
    set(MATH_COVERAGE 1)
    add_subdirectory(tests)
endif()

add_subdirectory(benchmarks)
