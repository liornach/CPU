// End-to-end integration test: loads the real ELF that
// `riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32` produced from
// fixtures/hello_world/main.c, runs it on our RV32I emulator, and checks that
// the memory-mapped UART received exactly "Hello, World!\n".
#include "cpu/cpu.hpp"
#include "elf/elf_loader.hpp"
#include "memory/memory.hpp"
#include "test_util.hpp"

#include <string>

#ifndef HELLO_WORLD_ELF_PATH
#error "HELLO_WORLD_ELF_PATH must be defined by the build (requires a RISC-V bare-metal GCC)"
#endif

int main()
{
    using ln::Cpu;
    using ln::ElfLoader;
    using ln::HaltReason;
    using ln::Memory;
    using namespace ln::test;

    // 1 MiB is comfortably larger than the fixture's own stack pointer (0x20000, set
    // by fixtures/hello_world/start.s), which is what actually matters here.
    Memory memory(1 * 1024 * 1024);

    std::string captured;
    memory.SetUartSink([&captured](char c) { captured += c; });

    auto file = ElfLoader::ReadFile(HELLO_WORLD_ELF_PATH);
    auto result = ElfLoader::Load(file, memory);

    Cpu cpu(memory);
    cpu.Reset(result.entryPoint, result.stackTop);
    HaltReason reason = cpu.Run(100000);

    Check(reason == HaltReason::Ebreak, "program halts cleanly via ebreak");
    Check(captured == "Hello, World!\n", "UART output is exactly \"Hello, World!\\n\"");

    return g_failures == 0 ? 0 : 1;
}
