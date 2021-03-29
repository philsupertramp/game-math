lint:
	find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\)' -exec clang-format -i {} \;

build-coverage:
	mkdir -p cmake-debug-coverage;
	./build.sh cmake-debug-coverage;

coverage:
	ctest --coverage --extra-verbose;
	lcov -c -d cmake-debug-coverage -o cov.info --include \*/include/math/\*;
	genhtml cov.info -o coverage-report;
