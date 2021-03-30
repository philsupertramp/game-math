lint:
	find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\)' -exec clang-format -i {} \;

test:
	./build.sh cmake-debug-coverage --test -o="MATH_EXTENSIONS=ds,numerics";

build-coverage:
	./build.sh cmake-debug-coverage;

coverage:
	./build.sh cmake-debug-coverage -c -o="MATH_EXTENSIONS=ds,numerics";
	lcov -c -d cmake-debug-coverage -o cov.info --include \*/include/math/\*;
	genhtml cov.info -o coverage-report;

minimal:
	./build.sh cmake-release-minimal -t=Release;

image-processing:
	./build.sh cmake-debug-coverage -c -o="MATH_EXTENSIONS=ds,numerics" -o="MATH_IMAGE_PROCESSING=1";