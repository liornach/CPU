#ifndef LN_CPU_CPU_HPP
#define LN_CPU_CPU_HPP

#include "components/adder32.hpp"
#include "components/bitwise32.hpp"
#include "components/comparator32.hpp"
#include "cpu/decoder.hpp"
#include "memory/memory.hpp"

#include <array>
#include <bitset>
#include <cstdint>

namespace ln
{

enum class HaltReason
{
    None,
    Ebreak,
    Ecall,
    UnknownInstruction,
    MaxInstructionsReached,
};

// RV32I fetch/decode/execute loop over 32 registers (x0 hardwired to 0) and a flat
// Memory. ADD/SUB/AND/OR/XOR/SLT/SLTU route through the gate-composed Adder32,
// Bitwise32, and Comparator32 components; shifts use native C++ shifts (no gate-level
// barrel shifter is implemented, since it wasn't required for any target program).
class Cpu
{
public:
    explicit Cpu(Memory& memory) : _memory(memory) {}

    void Reset(std::uint32_t entryPoint, std::uint32_t stackTop)
    {
        _regs.fill(0);
        _regs[2] = stackTop; // x2 = sp
        _pc = entryPoint;
        _halted = false;
        _haltReason = HaltReason::None;
    }

    // Executes instructions until halted or maxInstructions have run (a safety net
    // against runaway/incorrect programs looping forever).
    HaltReason Run(std::uint64_t maxInstructions)
    {
        std::uint64_t executed = 0;
        while (!_halted && executed < maxInstructions)
        {
            Step();
            ++executed;
        }
        if (!_halted)
        {
            _halted = true;
            _haltReason = HaltReason::MaxInstructionsReached;
        }
        return _haltReason;
    }

    void Step()
    {
        if (_halted) return;

        std::uint32_t word = _memory.LoadWord(_pc);
        DecodedInstruction inst = Decoder::Decode(word);

        std::uint32_t nextPc = _pc + 4;
        Execute(inst, nextPc);

        if (!_halted) _pc = nextPc;
    }

    std::uint32_t GetRegister(std::uint32_t index) const { return _regs.at(index); }
    std::uint32_t GetPc() const { return _pc; }
    bool IsHalted() const { return _halted; }
    HaltReason GetHaltReason() const { return _haltReason; }

private:
    void SetRegister(std::uint32_t index, std::uint32_t value)
    {
        if (index == 0) return; // x0 is hardwired to 0
        _regs.at(index) = value;
    }

    void Halt(HaltReason reason)
    {
        _halted = true;
        _haltReason = reason;
    }

    static std::bitset<32> B(std::uint32_t v) { return std::bitset<32>(v); }

    static std::uint32_t U32(std::bitset<32> bits)
    {
        std::uint32_t value = 0;
        for (int i = 31; i >= 0; --i) value = (value << 1) | (bits[i] ? 1u : 0u);
        return value;
    }

    static std::uint32_t SignExtendByte(std::uint8_t v)
    {
        return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(v)));
    }

    static std::uint32_t SignExtendHalf(std::uint16_t v)
    {
        return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(v)));
    }

    void Execute(const DecodedInstruction& inst, std::uint32_t& nextPc)
    {
        std::uint32_t rs1v = GetRegister(inst.rs1);
        std::uint32_t rs2v = GetRegister(inst.rs2);
        std::uint32_t imm = static_cast<std::uint32_t>(inst.imm);

        switch (inst.op)
        {
        case Op::LUI: SetRegister(inst.rd, imm); break;
        case Op::AUIPC: SetRegister(inst.rd, _pc + imm); break;

        case Op::JAL:
            SetRegister(inst.rd, _pc + 4);
            nextPc = _pc + imm;
            break;
        case Op::JALR:
            {
                std::uint32_t target = (rs1v + imm) & ~1u;
                SetRegister(inst.rd, _pc + 4);
                nextPc = target;
            }
            break;

        case Op::BEQ: if (rs1v == rs2v) nextPc = _pc + imm; break;
        case Op::BNE: if (rs1v != rs2v) nextPc = _pc + imm; break;
        case Op::BLT: if (Comparator32::LessThanSigned(B(rs1v), B(rs2v))) nextPc = _pc + imm; break;
        case Op::BGE: if (!Comparator32::LessThanSigned(B(rs1v), B(rs2v))) nextPc = _pc + imm; break;
        case Op::BLTU: if (Comparator32::LessThanUnsigned(B(rs1v), B(rs2v))) nextPc = _pc + imm; break;
        case Op::BGEU: if (!Comparator32::LessThanUnsigned(B(rs1v), B(rs2v))) nextPc = _pc + imm; break;

        case Op::LB: SetRegister(inst.rd, SignExtendByte(_memory.LoadByte(rs1v + imm))); break;
        case Op::LH: SetRegister(inst.rd, SignExtendHalf(_memory.LoadHalf(rs1v + imm))); break;
        case Op::LW: SetRegister(inst.rd, _memory.LoadWord(rs1v + imm)); break;
        case Op::LBU: SetRegister(inst.rd, _memory.LoadByte(rs1v + imm)); break;
        case Op::LHU: SetRegister(inst.rd, _memory.LoadHalf(rs1v + imm)); break;

        case Op::SB: _memory.StoreByte(rs1v + imm, static_cast<std::uint8_t>(rs2v)); break;
        case Op::SH: _memory.StoreHalf(rs1v + imm, static_cast<std::uint16_t>(rs2v)); break;
        case Op::SW: _memory.StoreWord(rs1v + imm, rs2v); break;

        case Op::ADDI: SetRegister(inst.rd, U32(Adder32::Compute(B(rs1v), B(imm), false).value)); break;
        case Op::SLTI: SetRegister(inst.rd, Comparator32::LessThanSigned(B(rs1v), B(imm)) ? 1u : 0u); break;
        case Op::SLTIU: SetRegister(inst.rd, Comparator32::LessThanUnsigned(B(rs1v), B(imm)) ? 1u : 0u); break;
        case Op::XORI: SetRegister(inst.rd, U32(Bitwise32::Xor(B(rs1v), B(imm)))); break;
        case Op::ORI: SetRegister(inst.rd, U32(Bitwise32::Or(B(rs1v), B(imm)))); break;
        case Op::ANDI: SetRegister(inst.rd, U32(Bitwise32::And(B(rs1v), B(imm)))); break;
        case Op::SLLI: SetRegister(inst.rd, rs1v << (imm & 0x1Fu)); break;
        case Op::SRLI: SetRegister(inst.rd, rs1v >> (imm & 0x1Fu)); break;
        case Op::SRAI: SetRegister(inst.rd, static_cast<std::uint32_t>(static_cast<std::int32_t>(rs1v) >> (imm & 0x1Fu))); break;

        case Op::ADD: SetRegister(inst.rd, U32(Adder32::Compute(B(rs1v), B(rs2v), false).value)); break;
        case Op::SUB: SetRegister(inst.rd, U32(Adder32::Compute(B(rs1v), B(rs2v), true).value)); break;
        case Op::SLL: SetRegister(inst.rd, rs1v << (rs2v & 0x1Fu)); break;
        case Op::SLT: SetRegister(inst.rd, Comparator32::LessThanSigned(B(rs1v), B(rs2v)) ? 1u : 0u); break;
        case Op::SLTU: SetRegister(inst.rd, Comparator32::LessThanUnsigned(B(rs1v), B(rs2v)) ? 1u : 0u); break;
        case Op::XOR: SetRegister(inst.rd, U32(Bitwise32::Xor(B(rs1v), B(rs2v)))); break;
        case Op::SRL: SetRegister(inst.rd, rs1v >> (rs2v & 0x1Fu)); break;
        case Op::SRA: SetRegister(inst.rd, static_cast<std::uint32_t>(static_cast<std::int32_t>(rs1v) >> (rs2v & 0x1Fu))); break;
        case Op::OR: SetRegister(inst.rd, U32(Bitwise32::Or(B(rs1v), B(rs2v)))); break;
        case Op::AND: SetRegister(inst.rd, U32(Bitwise32::And(B(rs1v), B(rs2v)))); break;

        case Op::ECALL: Halt(HaltReason::Ecall); break;
        case Op::EBREAK: Halt(HaltReason::Ebreak); break;

        case Op::UNKNOWN:
        default:
            Halt(HaltReason::UnknownInstruction);
            break;
        }
    }

    Memory& _memory;
    std::array<std::uint32_t, 32> _regs{};
    std::uint32_t _pc = 0;
    bool _halted = false;
    HaltReason _haltReason = HaltReason::None;
};

}

#endif
