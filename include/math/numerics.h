#pragma once

#include "Matrix.h"

#include "numerics/lin_alg/LU.h"
#include "numerics/lin_alg/backwardSub.h"
#include "numerics/lin_alg/forwardSub.h"
#include "numerics/lin_alg/gaussSeidel.h"
#include "numerics/lin_alg/newton.h"

#include "numerics/ode/ExplicitEuler.h"
#include "numerics/ode/ODESolver.h"
#include "numerics/ode/ode.h"
#include "numerics/ode/ode45.h"
#include "numerics/ode/odeBDF2.h"
#include "numerics/ode/odeTrapez.h"
