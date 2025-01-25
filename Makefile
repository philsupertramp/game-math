lint:
	find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\)' -not -path '*/pyDS/*' -exec clang-format -i {} \;

test:
	./build.sh cmake-debug-coverage --test;

test-release:
	./build.sh cmake-release-coverage --test -t=Release;

build-coverage:
	./build.sh cmake-debug-coverage;

coverage:
	./build.sh cmake-debug-coverage -c;
	lcov -c -d cmake-debug-coverage --rc branch_coverage=1 --rc function_coverage=1 -o test.info -f;
	lcov -a test.info -a base.info -o cov.info;
	genhtml --demangle-cpp --function-coverage --branch-coverage cov.info -o coverage-report;

minimal:
	./build.sh cmake-release-minimal -t=Release;

benchmark:
	./build.sh cmake-release -b --target=benchmarks -t=Release;
