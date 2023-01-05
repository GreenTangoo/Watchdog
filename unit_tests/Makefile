RES_TESTS_DIR=TestDir
SRC_TESTS_DIR=SrcTests
TEST_GEN_PATH=./cxxtest-4.4/bin/cxxtestgen
FLAGS_GEN=--error-printer



all: init tests postcompile

tests: SIEMStreamMemoryTest SIEMStreamFilesystemTest

SIEMStreamMemoryTest:
	$(TEST_GEN_PATH) $(FLAGS_GEN) -o runner.cpp $(SRC_TESTS_DIR)/SIEMStreamMemoryTest.h
	g++ -o $(RES_TESTS_DIR)/SIEMStreamMemoryTest -I cxxtest-4.4 -I ../utility_module runner.cpp

SIEMStreamFilesystemTest:
	$(TEST_GEN_PATH) $(FLAGS_GEN) -o runner.cpp $(SRC_TESTS_DIR)/SIEMStreamFilesystemTest.h
	g++ -o $(RES_TESTS_DIR)/SIEMStreamFilesystemTest -I cxxtest-4.4 -I ../utility_module runner.cpp

init:
	mkdir $(RES_TESTS_DIR)

postcompile:
	rm runner.cpp

clean:
	rm -rf $(RES_TESTS_DIR)
	if [ -f runner.cpp ]; then rm runner.cpp; fi
