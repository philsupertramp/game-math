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
        include/math/format.h
        include/math/format.cpp
)

if(MATH_EXTENSIONS MATCHES "(numerics)")
    set(LIB_SOURCES
            ${LIB_SOURCES}
            include/math/Matrix.h
            include/math/Matrix.cpp
            include/math/numerics/utils.h
            include/math/numerics/utils.cpp
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
            include/math/numerics/analysis/Spline.h
            include/math/numerics/analysis/Differentiation.h
            include/math/numerics/Fractals.h include/math/numerics/lin_alg/gaussJordan.h include/math/numerics/lin_alg/qr.h include/math/numerics/lin_alg/svd.h)
endif()
if(MATH_EXTENSIONS MATCHES "(symb)")
    set(LIB_SOURCES
        ${LIB_SOURCES}

        include/math/symb/Equation.h
        include/math/symb/Equation.cpp
        include/math/symb/MathNode.h
        include/math/symb/MathNode.cpp
        include/math/symb/Operator.cpp
        include/math/symb/Operator.h
        include/math/symb/Operand.cpp
        include/math/symb/Operand.h
        include/math/symb/EquationParser.cpp
        include/math/symb/EquationParser.h
            )
endif()
if (MATH_EXTENSIONS MATCHES "(ds)")
    set(LIB_SOURCES
        ${LIB_SOURCES}
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
            include/math/visualization/Plot.h
        )

    set(colorPallets gnpu magma inferno spectral)

    foreach(pallet IN LISTS colorPallets)
        configure_file(include/math/visualization/${pallet}.pal ${CMAKE_CURRENT_BINARY_DIR}/${pallet}.pal COPYONLY)
        configure_file(include/math/visualization/${pallet}.pal ${CMAKE_CURRENT_BINARY_DIR}/tests/${pallet}.pal COPYONLY)
    endforeach()

endif ()
if (MATH_EXTENSIONS MATCHES "(stat)")
    set(LIB_SOURCES ${LIB_SOURCES}
            include/math/statistics/Probability.h
            include/math/statistics/Probability.cpp
            include/math/statistics/Insurance.h
            include/math/statistics/Insurance.cpp
            include/math/statistics/Finance.h
            include/math/statistics/Finance.cpp
            )
endif ()

add_library(math-lib SHARED ${LIB_SOURCES})
set_target_properties(math-lib PROPERTIES LINKER_LANGUAGE CXX)


if (MATH_EXTENSIONS MATCHES "(ds)")
    target_link_libraries(math-lib ${ImageMagick_LIBRARIES})
endif()

if(MATH_TESTS)
    include(CTest)
    set(MATH_COVERAGE 1)
    add_subdirectory(tests)
endif()

add_subdirectory(benchmarks)