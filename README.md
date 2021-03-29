# game-math ![Tests](https://github.com/philsupertramp/game-math/workflows/CMake/badge.svg?branch=master)

Mathematical library for game developers.  
The library contains necessary elements of Matrix Algebra to enable working in 2D and 3D space.

# Installation

1. Download repository
`git clone git@github.com:/philsupertramp/game-math`

# Usage
see `tests/CMakeLists.txt` and `tests/main.cpp`

## Add extensions to library
Some extensions are optional, to activate them use
`MATH_EXTENSIONS` compile argument.

Example build command adding `numerics` module and building test suite:
```
mkdir -p build
cmake -DMATH_EXTENSIONS=numerics --build /home/phil/work/private/games/fawxy/src/vendor/math/build --target math-tests -- -j 3 .
```

# TOC
Module | Usage |
--- | --- |
`numerics/lin_alg` | Concepts of linear algebra solved using numerical methods |
`numerics/ode` | Solver for ordinary differential equations |
`ds` | Concepts of Data Science |

# Current feature support
- Math:
    - 2,3,4-dimensional Matrix/Vector algebra
- Game specific:
    - 2D/3D Transformation utils to generate View/Transformation matrices in a 2D view space
- Numerical Methods (extension `numerics`)
    - Solver for initial value problems of (stiff) ordinary differential equations (ode)
        - odeEulerExp – Explicit Euler Method `ExplicitEuler`
        - odeRK5      – explicit 5 step Runge-Kutta-Method `ode5`
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


# Development
Feel free to contribute to the project!

### How to contribute
- first time contributers
    - fork the project
    - create a PR based on your repository
- known contributers
    - create PR based on `master` branch

**NOTE: please run `make lint` prior to submitting your code!**  
This requires `clang-format>=11.0.0`
