#!/bin/bash

echo "Start string_manager_test"
cxxtestgen --error-printer -o runner.cpp string_manager_test.hpp
g++ -I . runner.cpp -o runner
./runner

echo "Start date_time_test"
cxxtestgen --error-printer -o runner.cpp date_time_test.hpp
g++ -I . runner.cpp -o runner
./runner

echo "Start json_test"
cxxtestgen --error-printer -o runner.cpp json_test.hpp
g++ -std=c++17 -I . runner.cpp -o runner -lstdc++fs
./runner

echo "Start description_table_test"
cxxtestgen --error-printer -o runner.cpp description_table_test.hpp
g++ -std=c++17 -I . runner.cpp -o runner
./runner

rm runner.cpp
rm runner
