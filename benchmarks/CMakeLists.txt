project(benchmarks)

cmake_minimum_required(VERSION 3.18.4)

include_directories(../include)

set(
        SOURCES
        main.cpp
)
link_libraries(math-lib)

add_executable(benchmarks ${SOURCES})

