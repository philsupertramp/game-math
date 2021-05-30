project(benchmarks)

include_directories(../include)

set(
        SOURCES
        main.cpp
)

add_executable(benchmarks ${SOURCES})

target_link_libraries(benchmarks math-lib)