// Test vectors are real instruction words taken from `riscv64-unknown-elf-gcc -march=rv32i
// -mabi=ilp32` output for the target Hello World program, cross-checked field-by-field with
// a reference decoding, so this test doubles as a check against the actual compiler output
// this emulator needs to run.
#include "cpu/decoder.hpp"
#include "test_util.hpp"

int main()
{
    using ln::Decoder;
    using ln::Op;
    using namespace ln::test;

    // lui sp, 0x20
    {
        auto d = Decoder::Decode(0x00020137u);
        Check(d.op == Op::LUI, "lui: op");
        Check(d.rd == 2, "lui: rd");
        Check(d.imm == 0x20000, "lui: imm");
    }

    // jal ra, +8 (call main)
    {
        auto d = Decoder::Decode(0x008000efu);
        Check(d.op == Op::JAL, "jal: op");
        Check(d.rd == 1, "jal: rd");
        Check(d.imm == 8, "jal: imm");
    }

    // j +0 (jal x0, 0) -- infinite loop after main returns
    {
        auto d = Decoder::Decode(0x0000006fu);
        Check(d.op == Op::JAL, "j: op");
        Check(d.rd == 0, "j: rd");
        Check(d.imm == 0, "j: imm");
    }

    // addi sp, sp, -32
    {
        auto d = Decoder::Decode(0xfe010113u);
        Check(d.op == Op::ADDI, "addi: op");
        Check(d.rd == 2, "addi: rd");
        Check(d.rs1 == 2, "addi: rs1");
        Check(d.imm == -32, "addi: imm");
    }

    // sw s0, 28(sp)
    {
        auto d = Decoder::Decode(0x00812e23u);
        Check(d.op == Op::SW, "sw: op");
        Check(d.rs1 == 2, "sw: rs1 (base)");
        Check(d.rs2 == 8, "sw: rs2 (source)");
        Check(d.imm == 28, "sw: imm (offset)");
    }

    // lbu a5, 0(a5)
    {
        auto d = Decoder::Decode(0x0007c783u);
        Check(d.op == Op::LBU, "lbu: op");
        Check(d.rd == 15, "lbu: rd");
        Check(d.rs1 == 15, "lbu: rs1");
        Check(d.imm == 0, "lbu: imm");
    }

    // sb a5, 0(a4)
    {
        auto d = Decoder::Decode(0x00f70023u);
        Check(d.op == Op::SB, "sb: op");
        Check(d.rs1 == 14, "sb: rs1 (base)");
        Check(d.rs2 == 15, "sb: rs2 (source)");
        Check(d.imm == 0, "sb: imm");
    }

    // bnez a5, -32  (bne a5, x0, -32)
    {
        auto d = Decoder::Decode(0xfe0790e3u);
        Check(d.op == Op::BNE, "bne: op");
        Check(d.rs1 == 15, "bne: rs1");
        Check(d.rs2 == 0, "bne: rs2");
        Check(d.imm == -32, "bne: imm");
    }

    // ret (jalr x0, 0(ra))
    {
        auto d = Decoder::Decode(0x00008067u);
        Check(d.op == Op::JALR, "ret: op");
        Check(d.rd == 0, "ret: rd");
        Check(d.rs1 == 1, "ret: rs1");
        Check(d.imm == 0, "ret: imm");
    }

    // Register-register ALU ops (hand-built, funct7 distinguishes ADD/SUB and SRL/SRA)
    Check(Decoder::Decode(0x003100b3u).op == Op::ADD, "add opcode/funct3/funct7=0"); // add x1,x2,x3
    Check(Decoder::Decode(0x403100b3u).op == Op::SUB, "sub opcode/funct3/funct7=0x20");
    Check(Decoder::Decode(0x003140b3u).op == Op::XOR, "xor funct3=4");
    Check(Decoder::Decode(0x003160b3u).op == Op::OR, "or funct3=6");
    Check(Decoder::Decode(0x003170b3u).op == Op::AND, "and funct3=7");
    Check(Decoder::Decode(0x003120b3u).op == Op::SLT, "slt funct3=2");
    Check(Decoder::Decode(0x003130b3u).op == Op::SLTU, "sltu funct3=3");
    Check(Decoder::Decode(0x003110b3u).op == Op::SLL, "sll funct3=1");
    Check(Decoder::Decode(0x003150b3u).op == Op::SRL, "srl funct3=5/funct7=0");
    Check(Decoder::Decode(0x403150b3u).op == Op::SRA, "sra funct3=5/funct7=0x20");

    return g_failures == 0 ? 0 : 1;
}
