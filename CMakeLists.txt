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

set(LIB_SOURCES
        include/math/math.h
        include/math/vec/vec2.h
        include/math/vec/vec3.h
        include/math/vec/vec4.h
        include/math/mat/mat2.h
        include/math/mat/mat3.h
        include/math/mat/mat4.h
        include/math/utils.h
        include/math/Random.h
)

if(MATH_EXTENSIONS MATCHES "(numerics)")
    set(LIB_SOURCES
            ${LIB_SOURCES}
            include/math/numerics.h
            include/math/Matrix.h
            include/math/numerics/utils.h
            include/math/numerics/utils.cpp
            include/math/numerics/lin_alg.h
            include/math/numerics/lin_alg/newton.h
            include/math/numerics/lin_alg/backwardSub.h
            include/math/numerics/lin_alg/forwardSub.h
            include/math/numerics/lin_alg/LU.h
            include/math/numerics/lin_alg/gaussSeidel.h
            include/math/numerics/ode.h
            include/math/numerics/ode/ODESolver.h
            include/math/numerics/ode/ExplicitEuler.h
            include/math/numerics/ode/ode.h
            include/math/numerics/ode/odeTrapez.h
            include/math/numerics/ode/ode45.h
            include/math/numerics/ode/odeBDF2.h

            include/math/numerics/analysis/SupportValues.h
            )
endif()
if(MATH_EXTENSION MATCHES "(symb)")
    set(LIB_SOURCES
        ${LIB_SOURCES}

        include/math/symb/Symbolic.h
    )
endif()
if (MATH_EXTENSIONS MATCHES "(ds)")
    set(LIB_SOURCES
        ${LIB_SOURCES}
            include/math/ds.h
            include/math/Matrix.h
            include/math/ds/DataSet.h
            include/math/ds/Classifier.h
            include/math/ds/Perceptron.h
            include/math/ds/AdalineGD.h
            include/math/ds/AdalineSGD.h
            include/math/ds/LogRegSGD.h
            include/math/ds/SGD.h
            )
    if(MATH_IMAGE_PROCESSING)
        set(LIB_SOURCES ${LIB_SOURCES}
                include/math/ds/ImageDataSet.h
        )
        add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
        add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
        find_package(ImageMagick COMPONENTS Magick++)
        include_directories(${ImageMagick_INCLUDE_DIRS})
    endif()
endif ()

if (MATH_EXTENSIONS MATCHES "(vis)")
    set(LIB_SOURCES ${LIB_SOURCES}
            include/math/visualization/plot.h
            include/math/visualization/Plot.h
        )
endif ()

add_library(math-lib SHARED ${LIB_SOURCES})
set_target_properties(math-lib PROPERTIES LINKER_LANGUAGE CXX)


if (MATH_EXTENSIONS MATCHES "(ds)")
    target_link_libraries(math-lib ${ImageMagick_LIBRARIES})
endif()

if(MATH_TESTS)
    include(CTest)
    add_subdirectory(tests)
endif()
