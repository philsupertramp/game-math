project(benchmarks)

cmake_minimum_required(VERSION 3.25)

include_directories(../include)
add_subdirectory(/home/phil/work/private/game-math/ cmake-debug)

set(
        SOURCES
        main.cpp
)
link_libraries(math-lib)

add_executable(benchmarks ${SOURCES})

