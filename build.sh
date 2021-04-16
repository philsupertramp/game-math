#!/usr/bin/env bash

set -e;
set -o;

DIR_NAME=$1

WITH_COVERAGE=0;
WITH_TESTS=0;
BUILD_TYPE="Debug";
BUILD_OPTIONS="";
TEST_ONLY=0;

for i in "$@"
do
  case $i in
    -c|--coverage)
      WITH_COVERAGE=1
      WITH_TESTS=1
      shift
      ;;
    --test)
      WITH_TESTS=1
      shift
      ;;
    --test-only|-to)
      TEST_ONLY=1
      shift
      ;;
    -t=*|--build-type=*)
      BUILD_TYPE="${i#*=}"
      shift
      ;;
    -o=*|--option=*)
      BUILD_OPTIONS="${BUILD_OPTIONS} -D${i#*=}"
      shift
      ;;
    *)
      ;;
esac
done

BUILD_OPTIONS_extension="-DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DMATH_SILENCE_WARNING=1 -DMATH_TESTS=${WITH_TESTS} -DMATH_COVERAGE=${WITH_COVERAGE}"

BUILD_OPTIONS="${BUILD_OPTIONS} ${BUILD_OPTIONS_extension}"

echo "Options: ${BUILD_OPTIONS}"

if [ ${TEST_ONLY} -eq 1 ]
then
  cd ${DIR_NAME};
  ctest --coverage --extra-verbose
else
  rm -rf ${DIR_NAME}
  mkdir -p ${DIR_NAME}
  (
    cd ${DIR_NAME};
    cmake ${BUILD_OPTIONS} -G "CodeBlocks - Unix Makefiles" ..
    cmake --build . -- -j 3;

    # Run test suite with or without coverage
    if [ ${WITH_COVERAGE:-0} == 1 ]; then
	    lcov -c -i -d . --rc lcov_branch_coverage=1 -o ../base.info --include \*/include/math/\*;
      ctest --coverage --extra-verbose
    else if [ ${WITH_TESTS:-0} == 1 ]; then
      ctest --extra-verbose
      fi
    fi
  )
fi