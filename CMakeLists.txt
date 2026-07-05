cmake_minimum_required(VERSION 3.10)
project(cpu_from_gates CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

set(GATE_TESTS
    nand_gate
    not_gate
    and_gate
    or_gate
    nor_gate
    xor_gate
)

foreach(gate ${GATE_TESTS})
    set(test_name test_${gate})
    add_executable(${test_name} tests/${test_name}.cpp)
    target_include_directories(${test_name} PRIVATE include tests)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
