#!/usr/bin/env bash

set -o;

DIR_NAME=$1
WITH_COVERAGE=1; [ "$2" == "-c" ] || WITH_COVERAGE=0;

mkdir -p ${DIR_NAME}
(
  cd ${DIR_NAME};
  cmake -DCMAKE_BUILD_TYPE=Debug -DMATH_SILENCE_WARNING=1 -DCMAKE_BUILD_TYPE=Debug -DMATH_COVERAGE=1 -DMATH_EXTENSIONS=numerics,ds -G "CodeBlocks - Unix Makefiles" ..
  cmake --build ${DIR_NAME};
  make -j 3 ;
  if [ ${WITH_COVERAGE:-0} == 1 ]; then
    ctest --coverage --extra-verbose
  fi
)
