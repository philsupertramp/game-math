# game-math

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


# Upcoming features
- Numerical Methods:
    - Newtons method for finding roots
    - More ode solver (see [here](http://www.netlib.org/ode/))
