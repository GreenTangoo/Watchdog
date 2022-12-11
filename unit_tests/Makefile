RES_TESTS_DIR=TestDir
SRC_TESTS_DIR=SrcTests
TEST_GEN_PATH=./cxxtest-4.4/bin/cxxtestgen
FLAGS_GEN=--error-printer



all: clean init tests postcompile

tests: JsonStreamTest

JsonStreamTest:
	$(TEST_GEN_PATH) $(FLAGS_GEN) -o runner.cpp $(SRC_TESTS_DIR)/JsonStreamTest.h
	g++ -o $(RES_TESTS_DIR)/JsonStreamTest -I cxxtest-4.4 runner.cpp

init:
	mkdir $(RES_TESTS_DIR)

postcompile:
	rm runner.cpp

clean:
	rm -rf $(RES_TESTS_DIR)
