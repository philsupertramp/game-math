# numerics
split into submodules

## /lin_alg/  
Contains newton-algorithm and Gauss-Seidel-Algorithm to solve systems of linear equations.  
Also contains LU-substitution.

## /ode/  
ODESolver.h contains several solvers for ordinary differential equations.    
Hint! If you wanna approximate a pde (partial differential equation) just transform it into a system of ode's and use
BDF(2) ;)

## /analysis/

Contains `SupportValues.h` and `NaturalSpline.h`  
both methods to interpolate a function by given support values.  

Famous example by runge  

![spline-interpol-runge](https://user-images.githubusercontent.com/9550040/118409523-1ee58600-b68b-11eb-863e-5ed136a24bb4.png)
