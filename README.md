# game-math 
![Tests](https://github.com/philsupertramp/game-math/workflows/CMake/badge.svg?branch=master) [![CodeFactor](https://www.codefactor.io/repository/github/philsupertramp/game-math/badge)](https://www.codefactor.io/repository/github/philsupertramp/game-math)

Mathematical library for game developers.  
The library contains necessary elements of Matrix Algebra to enable working in 2D and 3D space.

# Current feature support
- Math:
  - 2,3,4-dimensional Matrix/Vector algebra (math.h)
- Game specific:
  - 2D/3D Transformation utils to generate View/Transformation matrices in a 2D view space (utils.h)
- Numerical Methods (extension `numerics`)
  - Solver for initial value problems of (stiff) ordinary differential equations (ode.h)
    - Unified class for solvers (ODESolver)
    - Explicit Euler Method (ExplicitEuler.h)
    - Explicit 5 step Runge-Kutta-Method (ode45.h)
    - Trapezoid rule for odes (odeTrapez.h)
    - Backward differential formula (odeBDF2.h)
  - Solver for systems of linear equations (gaussSeidel.h)
  - Gauss-Jordan method to calculate inverse matrices (gaussJordan.h)
  - QR-Decomposition of matrices (qr.h)
  - Singular Value Decomposition (SVD) (svd.h)
  - Fractals using numerical approximations (Fractals.h)
    - NewtonFractal
    - Mandelbrot
  - Newton method to approximate the zero-value for a given function based on an initial value newton.h
  - Function Interpolation/Approximation
    - 1D Interpolation
      - Polynomial Interpolation (PolynomialBase)
        - MonomBase
        - LagrangeBase
        - NewtonBase
      - Spline Interpolation
        - Spline: implements Natural cubic spline, as well as a B-Spline capable of interpolating 3D values 
    - 2D/3D Interpolation
      - see Spline
  - Differential calculus (Differentiation.h)
  - Numerical Integration (Integration.h)
- (classic) Statistics:
  - Probability.h
  - Insurance.h
- Plot support (uses/requires gnuplot see include/math/visualization/README.md or Plot.h)
- Data Science:
  - Neural networks
    - Feed Forward NNs
      - Perceptron classifier (`Perceptron`)
      - Adaline Neuron Classifier
        - using gradient decent method (`AdalineGD`)
        - using statistics gradient decent method (`AdalineSGD`)

# Benchmarks
see `/benchmarks/README.md`
```
Matrix<double>:
finished computation with 9 elements and 5000000 * operations. elapsed time: 1.13369s
finished computation with 9 elements and 5000000 * operations. elapsed time: 0.115082s
finished computation with 9 elements and 5000000 + operations. elapsed time: 1.16305s
mat3<double>:
finished computation with 9 elements and 5000000 * operations. elapsed time: 6.5e-08s
finished computation with 9 elements and 5000000 + operations. elapsed time: 5.2e-08s
```

#### Build requirements
`gcc, cmake, clang`

# Installation

1. Download repository
`git clone git@github.com:/philsupertramp/game-math`

2. Integration into your project

**Only CMake fully tested**
### cmake:
in your `CMakeLists.txt` add
```cmake
link_libraries(math-lib)
include_directories(math/include)
```

# Usage

After linking the library correctly one is able to include
the main header file `math.h`
```c++
#include <math/math.h>
```
Although one can explicitly include parts
```c++
#include <math/vec/vec2.h>
```

## Extensions
Currently there are two different implementations
of matrices/vectors
### Implementation 1)
Game dev oriented. 2, 3 and 4 dimensional vector/matrix
implementations.  
Tightly packed data to allow minimal memory usage.
see `./include/math/vec` and `./include/math/mat`
for implementation details.

### Implementation 2)
`Matrix<T>` (see `./include/math/Matrix.h`) represents a `m x n`-Matrix of data of type `T`

a specific data science and numerics implementation.  
Capable of Matrix-Vector and Matrix-Multiplication, it also contains
data science specific algorithms and statistical methods.

# Extensions
The library is split into multiple content seperated module.
Each module is nested in the root directory [`math/`](/include/math).  
Each sub-directory contains a README file with instructions how to use the module
and if it has dependencies.

### Adding extensions to library
To activate modules use the `MATH_EXTENSIONS` compile argument.

Example build command adding `numerics` module and building test suite:
```
mkdir -p build
cmake -DMATH_EXTENSIONS=numerics --build math/build -- -j 3 .
```

# Development
Feel free to contribute to the project!

### How to contribute
- first time contributors
    - fork the project
    - create a PR based on your repository
- known contributors
    - create PR based on `master` branch

**NOTE: please run `make lint` prior to submitting your code!**  
This requires `clang-format>=11.0.0`

# License
The project is under `MIT`-License see `LICENSE` for more

## Acknowledgements

