// Minimal command-line front end: loads a bare-metal RV32I ELF and runs it,
// printing whatever the program writes to the memory-mapped UART to stdout.
//
// Usage: rv32i_emulator <path-to-elf> [max-instructions]
#include "cpu/cpu.hpp"
#include "elf/elf_loader.hpp"
#include "memory/memory.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: " << argv[0] << " <path-to-elf> [max-instructions]\n";
        return 2;
    }

    std::uint64_t maxInstructions = argc >= 3 ? std::strtoull(argv[2], nullptr, 10) : 1'000'000ull;

    ln::Memory memory(1 * 1024 * 1024);
    memory.SetUartSink([](char c) { std::cout << c << std::flush; });

    ln::ElfLoadResult loaded;
    try
    {
        auto file = ln::ElfLoader::ReadFile(argv[1]);
        loaded = ln::ElfLoader::Load(file, memory);
    }
    catch (const std::exception& e)
    {
        std::cerr << "failed to load ELF: " << e.what() << "\n";
        return 1;
    }

    ln::Cpu cpu(memory);
    cpu.Reset(loaded.entryPoint, loaded.stackTop);
    ln::HaltReason reason = cpu.Run(maxInstructions);

    switch (reason)
    {
    case ln::HaltReason::Ebreak:
    case ln::HaltReason::Ecall:
        return 0;
    case ln::HaltReason::UnknownInstruction:
        std::cerr << "halted: unknown instruction at pc=0x" << std::hex << cpu.GetPc() << "\n";
        return 1;
    case ln::HaltReason::MaxInstructionsReached:
        std::cerr << "halted: hit the " << std::dec << maxInstructions << "-instruction cap\n";
        return 1;
    default:
        return 1;
    }
}
