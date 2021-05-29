lint:
	find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\)' -exec clang-format -i {} \;

test:
	./build.sh cmake-debug-coverage --test -o="MATH_EXTENSIONS=ds,numerics,symb,stat";

test-release:
	./build.sh cmake-release-coverage --test -t=Release -o="MATH_EXTENSIONS=ds,numerics,symb,stat";

build-coverage:
	./build.sh cmake-debug-coverage;

coverage:
	./build.sh cmake-debug-coverage -c -o="MATH_EXTENSIONS=numerics,ds,stat,visualization -DMATH_IMAGE_PROCESSING=1";
	lcov -c -d cmake-debug-coverage --rc lcov_branch_coverage=1 -o test.info -f --include \*/include/math/\* --include \*/include/math/numerics\* --include \*/math/numerics/lin_alg\* --include \*/math/numerics/ode\* --include \*/math/statistics/\*;
	lcov -a test.info -a base.info -o cov.info;
	genhtml cov.info -o coverage-report;

test-coverage:
	./build.sh cmake-debug-coverage -c -o="MATH_EXTENSIONS=numerics,ds,vis,symb,stat" --target=plot_TestPlot;
	lcov -c -d cmake-debug-coverage --rc lcov_branch_coverage=1 -o test.info -f --include \*/include/math/\* --include \*/include/math/numerics\* --include \*/math/numerics/lin_alg\* --include \*/math/numerics/ode\* --include \*/math/statistics/\*;
	lcov -a test.info -a base.info -o cov.info;
	genhtml cov.info -o coverage-report;

minimal:
	./build.sh cmake-release-minimal -t=Release;

image-processing:
	./build.sh cmake-debug-coverage -c -o="MATH_EXTENSIONS=ds,numerics" -o="MATH_IMAGE_PROCESSING=1";
