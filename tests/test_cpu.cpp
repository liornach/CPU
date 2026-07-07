// Program below is hand-assembled RV32I (encoded with a small throwaway Python
// assembler, whose field encodings were cross-checked against test_decoder.cpp's
// real GCC-derived vectors), covering arithmetic, branches, jumps, and load/store.
#include "cpu/cpu.hpp"
#include "memory/memory.hpp"
#include "test_util.hpp"

#include <cstdint>
#include <vector>

int main()
{
    using ln::Cpu;
    using ln::HaltReason;
    using ln::Memory;
    using namespace ln::test;

    std::vector<std::uint32_t> program = {
        0x00500093u, // addi x1, x0, 5
        0x00300113u, // addi x2, x0, 3
        0x002081b3u, // add  x3, x1, x2      -> 8
        0x40208233u, // sub  x4, x1, x2      -> 2
        0x0020f2b3u, // and  x5, x1, x2      -> 1
        0x0020e333u, // or   x6, x1, x2      -> 7
        0x0020c3b3u, // xor  x7, x1, x2      -> 6
        0x00112433u, // slt  x8, x2, x1      -> 1 (3 < 5)
        0x0020b4b3u, // sltu x9, x1, x2      -> 0 (5 < 3 unsigned is false)
        0x00108463u, // beq  x1, x1, +8      -> taken, skips next instr
        0x06f00513u, // addi x10, x0, 111    -> skipped
        0x0de00513u, // addi x10, x0, 222    -> branch target, x10 = 222
        0x008005efu, // jal  x11, +8         -> x11 = return addr, jump +8
        0x3e700613u, // addi x12, x0, 999    -> skipped
        0x02a00693u, // addi x13, x0, 42     -> jal target, x13 = 42
        0x06d02223u, // sw   x13, 100(x0)
        0x06402703u, // lw   x14, 100(x0)    -> x14 = 42
        0x00100073u, // ebreak
    };

    Memory memory(4096);
    for (std::size_t i = 0; i < program.size(); ++i)
    {
        memory.StoreWord(static_cast<std::uint32_t>(i * 4), program[i]);
    }

    Cpu cpu(memory);
    cpu.Reset(0, 4096);
    HaltReason reason = cpu.Run(1000);

    Check(reason == HaltReason::Ebreak, "program halts on ebreak");

    Check(cpu.GetRegister(1) == 5, "x1 == 5");
    Check(cpu.GetRegister(2) == 3, "x2 == 3");
    Check(cpu.GetRegister(3) == 8, "x3 = 5 + 3 == 8");
    Check(cpu.GetRegister(4) == 2, "x4 = 5 - 3 == 2");
    Check(cpu.GetRegister(5) == 1, "x5 = 5 & 3 == 1");
    Check(cpu.GetRegister(6) == 7, "x6 = 5 | 3 == 7");
    Check(cpu.GetRegister(7) == 6, "x7 = 5 ^ 3 == 6");
    Check(cpu.GetRegister(8) == 1, "x8 = (3 < 5) == 1");
    Check(cpu.GetRegister(9) == 0, "x9 = (5 < 3 unsigned) == 0");
    Check(cpu.GetRegister(10) == 222, "beq branch taken, skipped instruction did not execute");
    Check(cpu.GetRegister(11) == 0x34, "jal wrote the correct return address");
    Check(cpu.GetRegister(12) == 0, "jal target skipped the instruction after it (x12 untouched)");
    Check(cpu.GetRegister(13) == 42, "jal landed on the correct target");
    Check(cpu.GetRegister(14) == 42, "load reads back what was stored");

    Check(cpu.GetRegister(0) == 0, "x0 stays 0");

    // x0 is hardwired: writes to it must be silently discarded.
    {
        Memory mem2(64);
        // addi x0, x0, 5 ; ebreak
        mem2.StoreWord(0, 0x00500013u);
        mem2.StoreWord(4, 0x00100073u);
        Cpu cpu2(mem2);
        cpu2.Reset(0, 64);
        cpu2.Run(10);
        Check(cpu2.GetRegister(0) == 0, "writes to x0 are discarded");
    }

    // Unknown instruction halts rather than executing garbage.
    {
        Memory mem3(64);
        mem3.StoreWord(0, 0xFFFFFFFFu); // not a valid RV32I encoding
        Cpu cpu3(mem3);
        cpu3.Reset(0, 64);
        HaltReason r3 = cpu3.Run(10);
        Check(r3 == HaltReason::UnknownInstruction, "unknown instruction halts the CPU");
    }

    // Max instruction count acts as a safety net against infinite loops.
    {
        Memory mem4(64);
        mem4.StoreWord(0, 0x0000006fu); // jal x0, 0 -- infinite loop
        Cpu cpu4(mem4);
        cpu4.Reset(0, 64);
        HaltReason r4 = cpu4.Run(50);
        Check(r4 == HaltReason::MaxInstructionsReached, "runaway loop stops at the instruction cap");
    }

    return g_failures == 0 ? 0 : 1;
}
