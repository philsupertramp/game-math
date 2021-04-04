lint:
	find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\)' -exec clang-format -i {} \;

test:
	./build.sh cmake-debug-coverage --test -o="MATH_EXTENSIONS=ds,numerics";

build-coverage:
	./build.sh cmake-debug-coverage;

coverage:
	./build.sh cmake-debug-coverage -c -o="MATH_EXTENSIONS=numerics,ds";
	lcov -c -d cmake-debug-coverage --rc lcov_branch_coverage=1 -o test.info --include \*/math/\*;
	lcov -a test.info -a base.info -o cov.info;
	genhtml cov.info -o coverage-report;

minimal:
	./build.sh cmake-release-minimal -t=Release;

image-processing:
	./build.sh cmake-debug-coverage -c -o="MATH_EXTENSIONS=ds,numerics" -o="MATH_IMAGE_PROCESSING=1";