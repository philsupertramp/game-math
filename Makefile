lint:
	find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\)' -exec clang-format -i {} \;

coverage:
	mkdir -p cmake-debug-coverage;
	./build.sh cmake-debug-coverage;
	lcov -c -d cmake-debug-coverage -o cov.info --include \*/include/math/\*;
	genhtml cov.info -o coverage-report;
