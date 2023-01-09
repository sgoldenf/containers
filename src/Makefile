.PHONY: all clean test test.out
SHELL = /bin/sh
OS = $(shell uname)
CC = g++
STD = -std=c++17
GCOV = -fprofile-arcs -ftest-coverage

ifeq ($(OS), Linux)
	GTEST_LIB = -lgtest -lpthread
else
	GTEST_LIB = -lgtest
endif

GCOV_COMPILE_FLAGS  = -fprofile-arcs -ftest-coverage
GCOV_RESULT	= *.gcda *.gcno *.gcov
GCOV 			  = gcov
GCOV_FLAGS  = -kjr

LCOV          = lcov
LCOV_FLAGS    = --no-external -c -d .
COVERAGE_INFO = coverage.info

all: report

test:
	$(RM) $(GCOV_RESULT)
	$(CC) $(STD) $(GCOV_COMPILE_FLAGS) tests.cpp $(GTEST_LIB) -o test.out
	./test.out

report: test
	$(GCOV) $(GCOV_FLAGS) *.h
	$(LCOV) $(LCOV_FLAGS) -o $(COVERAGE_INFO)
	genhtml $(COVERAGE_INFO) -o report

clean:
	rm -rf ./report
	rm -f *.gcno *.gcda *.info test.out *.o *.a *.txt

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --log-file=log.txt ./test.out

leaks:
	CK_FORK=no leaks --atExit -- ./test.out

format:
	clang-format -n *.h ./tests/*.cpp

cppcheck:
	cppcheck --language=c++ --std=c++17 --enable=all --suppress=missingInclude --suppress=unusedFunction --suppress=useStlAlgorithm  *.h ./tests/*.cpp
