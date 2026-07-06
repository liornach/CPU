cmake_minimum_required(VERSION 3.10)
project(cpu_from_gates CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(rv32i_emulator tools/rv32i_emulator.cpp)
target_include_directories(rv32i_emulator PRIVATE include)

set(GATE_TESTS
    nand_gate
    not_gate
    and_gate
    or_gate
    nor_gate
    xor_gate
)

set(COMPONENT_TESTS
    half_adder
    full_adder
    mux2
    bitwise32
    adder32
    comparator32
    sign_extend
)

set(MEMORY_TESTS
    memory
    elf_loader
)

set(CPU_TESTS
    decoder
    cpu
)

foreach(name ${GATE_TESTS} ${COMPONENT_TESTS} ${MEMORY_TESTS} ${CPU_TESTS})
    set(test_name test_${name})
    add_executable(${test_name} tests/${test_name}.cpp)
    target_include_directories(${test_name} PRIVATE include tests)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# End-to-end Hello World integration test: compiles fixtures/hello_world/main.c with a
# bare-metal RISC-V GCC and runs the resulting ELF on the emulator. Skipped when no such
# toolchain is available, since it's an external dependency the rest of the suite doesn't need.
find_program(RISCV_GCC NAMES riscv64-unknown-elf-gcc riscv32-unknown-elf-gcc)

if(RISCV_GCC)
    set(HELLO_WORLD_DIR ${CMAKE_SOURCE_DIR}/fixtures/hello_world)
    set(HELLO_WORLD_ELF ${CMAKE_BINARY_DIR}/hello_world.elf)
    set(RISCV_FLAGS -march=rv32i -mabi=ilp32 -nostdlib -ffreestanding -O0)

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/hello_world_main.o
        COMMAND ${RISCV_GCC} ${RISCV_FLAGS} -c ${HELLO_WORLD_DIR}/main.c -o ${CMAKE_BINARY_DIR}/hello_world_main.o
        DEPENDS ${HELLO_WORLD_DIR}/main.c
        COMMENT "Compiling hello_world fixture main.c for RV32I"
    )
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/hello_world_start.o
        COMMAND ${RISCV_GCC} ${RISCV_FLAGS} -c ${HELLO_WORLD_DIR}/start.s -o ${CMAKE_BINARY_DIR}/hello_world_start.o
        DEPENDS ${HELLO_WORLD_DIR}/start.s
        COMMENT "Assembling hello_world fixture start.s for RV32I"
    )
    add_custom_command(
        OUTPUT ${HELLO_WORLD_ELF}
        COMMAND ${RISCV_GCC} ${RISCV_FLAGS} -T ${HELLO_WORLD_DIR}/link.ld
                ${CMAKE_BINARY_DIR}/hello_world_start.o ${CMAKE_BINARY_DIR}/hello_world_main.o
                -o ${HELLO_WORLD_ELF}
        DEPENDS ${CMAKE_BINARY_DIR}/hello_world_start.o ${CMAKE_BINARY_DIR}/hello_world_main.o ${HELLO_WORLD_DIR}/link.ld
        COMMENT "Linking hello_world.elf"
    )
    add_custom_target(hello_world_elf DEPENDS ${HELLO_WORLD_ELF})

    add_executable(test_hello_world tests/test_hello_world.cpp)
    target_include_directories(test_hello_world PRIVATE include tests)
    target_compile_definitions(test_hello_world PRIVATE HELLO_WORLD_ELF_PATH="${HELLO_WORLD_ELF}")
    add_dependencies(test_hello_world hello_world_elf)
    add_test(NAME test_hello_world COMMAND test_hello_world)
else()
    message(STATUS "riscv64-unknown-elf-gcc / riscv32-unknown-elf-gcc not found; skipping the Hello World integration test")
endif()
