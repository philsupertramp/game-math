# game-math

Mathematical library for game developers.  
The library contains necessary elements of of Matrix Algebra to enable working in 2D and 3D space.  


# Installation

1. Download repository
`git clone git@github.com:/philsupertramp/game-math`

# Usage
see `tests/CMakeLists.txt` and `tests/main.cpp`


# Current feature support
- Math:
    - 2,3,4-dimensional Matrix/Vector algebra
- Game specific:
    - 2D/3D Transformation utils to generate View/Transformation matrices in a 2D view space
- Numerical Methods
    - Solver for initial value problems of (stiff) ordinary differential equations (ode)
        - odeEulerExp – Explicit Euler Method
        - odeRK5      – explicit 5 step Runge-Kutta-Method 


# Upcoming features
- Numerical Methods:
    - Newtons method for finding roots
    - More ode solver (see [here](http://www.netlib.org/ode/))
