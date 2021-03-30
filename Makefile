lint:
	find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\)' -exec clang-format -i {} \;

build-coverage:
	./build.sh cmake-debug-coverage;

coverage:
	./build.sh cmake-debug-coverage -c;
	lcov -c -d cmake-debug-coverage -o cov.info --include \*/include/math/\*;
	genhtml cov.info -o coverage-report;
