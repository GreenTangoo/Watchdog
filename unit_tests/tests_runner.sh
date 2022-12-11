#!/bin/bash

# Run all tests from TestDir project folder

YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NOCOLOR='\033[0m'

TEST_DIR=TestDir

for file in $TEST_DIR/*; do
  echo -e "${YELLOW}Running${NOCOLOR} ${BLUE}$file${NOCOLOR} ${YELLOW}test${NOCOLOR}";

  ./$file;
done
