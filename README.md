# game-math 
![Tests](https://github.com/philsupertramp/game-math/workflows/CMake/badge.svg?branch=master) [![CodeFactor](https://www.codefactor.io/repository/github/philsupertramp/game-math/badge)](https://www.codefactor.io/repository/github/philsupertramp/game-math)

Mathematical library for game developers.  
The library contains necessary elements of Matrix Algebra to enable working in 2D and 3D space.

# Current feature support
- Math:
  - 2,3,4-dimensional Matrix/Vector algebra (math.h)
  - Matrix class with many implemented opterations
- Game specific:
  - 2D/3D Transformation utils to generate View/Transformation matrices in a 2D view space (utils.h)
- Sorting (insertion sort)
- Random Number Generator
- String formatting

# Benchmarks
see `/benchmarks/README.md`
```
Matrix<int>:
finished computation with 9 elements and 5000000 * operations. elapsed time: 0.160372s
finished computation with 9 elements and 5000000 # operations. elapsed time: 0.0172873s
finished computation with 9 elements and 5000000 + operations. elapsed time: 0.145237s
mat3<int>:
finished computation with 9 elements and 5000000 * operations. elapsed time: 4.3e-08s
finished computation with 9 elements and 5000000 + operations. elapsed time: 5e-08s
Matrix<double>:
finished computation with 9 elements and 5000000 * operations. elapsed time: 0.132516s
finished computation with 9 elements and 5000000 # operations. elapsed time: 0.00899579s
finished computation with 9 elements and 5000000 + operations. elapsed time: 0.152282s
mat3<double>:
finished computation with 9 elements and 5000000 * operations. elapsed time: 5.2e-08s
finished computation with 9 elements and 5000000 + operations. elapsed time: 5e-08s
Matrix<float>:
finished computation with 9 elements and 5000000 * operations. elapsed time: 0.186274s
finished computation with 9 elements and 5000000 # operations. elapsed time: 0.00887903s
finished computation with 9 elements and 5000000 + operations. elapsed time: 0.146145s
mat3<float>:
finished computation with 9 elements and 5000000 * operations. elapsed time: 7.1e-08s
finished computation with 9 elements and 5000000 + operations. elapsed time: 8e-08s
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

