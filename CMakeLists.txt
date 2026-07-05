cmake_minimum_required(VERSION 3.10)
project(cpu_from_gates CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(test_nand_gate tests/test_nand_gate.cpp)
target_include_directories(test_nand_gate PRIVATE include)

add_test(NAME test_nand_gate COMMAND test_nand_gate)
