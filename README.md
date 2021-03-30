# game-math ![Tests](https://github.com/philsupertramp/game-math/workflows/CMake/badge.svg?branch=master)

Mathematical library for game developers.  
The library contains necessary elements of Matrix Algebra to enable working in 2D and 3D space.

# Current feature support
- Math:
  - 2,3,4-dimensional Matrix/Vector algebra
- Game specific:
  - 2D/3D Transformation utils to generate View/Transformation matrices in a 2D view space
- Numerical Methods (extension `numerics`)
  - Solver for initial value problems of (stiff) ordinary differential equations (ode)
    - Explicit Euler Method (`odeEulerExp`)
    - Explicit 5 step Runge-Kutta-Method (`ode45`)
  - Solver for systems of linear equations `gaussianElimination`
- Data Science:
  - Neural networks
    - Feed Forward NNs
      - Perceptron classifier
      - Adaline Neuron Classifier
        - using gradient decent method
        - using statistics gradient decent method
# Upcoming features
- Numerical Methods:
  - Newtons method for finding roots
  - More ode solver (see [here](http://www.netlib.org/ode/))

# Installation

1. Download repository
`git clone git@github.com:/philsupertramp/game-math`

2. Integration into your project

**Only CMake fully tested**
### cmake:
in your `CMakeLists.txt` add
```cmake
link_libraries(math)
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
### Available extensions:
#### `numerics`: Numerical methods for linear algebra and odes
```c++
#include <math/numerics/lin_alg.h>
#include <math/numerics/ode.h>
```
#### `ds`: Methods out of the field of data science
```c++
#include <math/ds.h>
```
#### TOC
Module | Usage |
--- | --- |
`numerics/lin_alg` | Concepts of linear algebra solved using numerical methods |
`numerics/ode` | Solver for ordinary differential equations |
`ds` | Concepts of Data Science |

### Add extensions to library
Some extensions are optional, to activate them use
`MATH_EXTENSIONS` compile argument.

Example build command adding `numerics` module and building test suite:
```
mkdir -p build
cmake -DMATH_EXTENSIONS=numerics --build math/build -- -j 3 .
```

### Note, some extensions or parts of them require specific dependencies.
Look into module `README.md` files.

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
