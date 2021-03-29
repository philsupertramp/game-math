#!/usr/bin/env bash

set -x;

DIR_NAME=$1

mkdir -p ${DIR_NAME}

(
  cd cmake-debug-coverage/;
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_BUILD_TYPE=Debug -DMATH_COVERAGE=1 -DMATH_EXTENSIONS=numerics,ds -G "CodeBlocks - Unix Makefiles" ..
  cmake --build ${DIR_NAME};
  make -j 3 ;
  ctest --coverage --extra-verbose;
)
