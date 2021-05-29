#!/usr/bin/env sh

set -e;
set -o;

DIR_NAME=$1

WITH_COVERAGE=0;
WITH_TESTS=0;
BUILD_TYPE="Debug";
BUILD_OPTIONS="";
TEST_ONLY=0;
CLEAR=0;
SPECIFIC_TEST=0;
TARGET="";

for i in "$@"
do
  case $i in
    -c|--coverage)
      WITH_COVERAGE=1;
      WITH_TESTS=1;
      echo "Purging coverage files..."
      rm -rf $(find ${DIR_NAME} \( -iname '*.gcda' \))
      shift
      ;;
    --clear)
      CLEAR=1
      shift
      ;;
    --test)
      WITH_TESTS=1
      shift
      ;;
    --target=*)
      WITH_TESTS=1
      SPECIFIC_TEST="${i#*=}"
      TARGET="--target ${SPECIFIC_TEST}"
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

function test_command() {
  COMMAND="ctest --extra-verbose --output-on-failure"

  # Run test suite with or without coverage
  if [ ${WITH_COVERAGE:-0} == 1 ]; then
	  COMMAND="${COMMAND} --coverage"
    lcov -c -i -d . --rc lcov_branch_coverage=1 -o ../base.info --include \*/include/math/\* --include \*/include/math/numerics\* --include \*/math/numerics/lin_alg\* --include \*/math/numerics/ode\* --include \*/math/statistics/\*;
  fi
  if [ "${SPECIFIC_TEST:-0}" != "0" ]; then
      COMMAND="${COMMAND} -R ${SPECIFIC_TEST}"
  fi

  eval "${COMMAND}"
}

BUILD_OPTIONS_extension="-DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DMATH_SILENCE_WARNING=1 -DMATH_TESTS=${WITH_TESTS} -DMATH_COVERAGE=${WITH_COVERAGE}"

BUILD_OPTIONS="${BUILD_OPTIONS} ${BUILD_OPTIONS_extension}"

echo "Options: ${BUILD_OPTIONS}"

if [ ${TEST_ONLY} -eq 1 ]
then
  cd ${DIR_NAME};
  ctest --coverage --extra-verbose
else
  if [ ${CLEAR:-0} == 1 ]; then
    rm -rf ${DIR_NAME};
  fi
  mkdir -p ${DIR_NAME}
  (
    cd ${DIR_NAME};
    cmake ${BUILD_OPTIONS} -G "CodeBlocks - Unix Makefiles" ..
    cmake --build . ${TARGET} -- -j 3;

    test_command
  )
fi