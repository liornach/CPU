#ifndef LN_CPU_DECODER_HPP
#define LN_CPU_DECODER_HPP

#include "components/sign_extend.hpp"

#include <bitset>
#include <cstdint>

namespace ln
{

enum class Op
{
    LUI, AUIPC,
    JAL, JALR,
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    LB, LH, LW, LBU, LHU,
    SB, SH, SW,
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,
    ECALL, EBREAK,
    UNKNOWN
};

struct DecodedInstruction
{
    Op op = Op::UNKNOWN;
    std::uint32_t rd = 0;
    std::uint32_t rs1 = 0;
    std::uint32_t rs2 = 0;
    std::int32_t imm = 0; // meaning depends on op: branch/jump offset, load/store offset, ALU operand, or shift amount
    std::uint32_t raw = 0;
};

// Decodes RV32I instruction words (R/I/S/B/U/J formats). Covers the base integer ISA
// minus FENCE, which a bare-metal single-hart emulator has no need to act on.
class Decoder
{
public:
    static DecodedInstruction Decode(std::uint32_t word)
    {
        DecodedInstruction inst;
        inst.raw = word;

        std::uint32_t opcode = word & 0x7Fu;
        inst.rd = (word >> 7) & 0x1Fu;
        std::uint32_t funct3 = (word >> 12) & 0x7u;
        inst.rs1 = (word >> 15) & 0x1Fu;
        inst.rs2 = (word >> 20) & 0x1Fu;
        std::uint32_t funct7 = (word >> 25) & 0x7Fu;

        switch (opcode)
        {
        case 0x37: inst.op = Op::LUI; inst.imm = static_cast<std::int32_t>(word & 0xFFFFF000u); break;
        case 0x17: inst.op = Op::AUIPC; inst.imm = static_cast<std::int32_t>(word & 0xFFFFF000u); break;

        case 0x6F: inst.op = Op::JAL; inst.imm = DecodeJImm(word); break;
        case 0x67: inst.op = Op::JALR; inst.imm = DecodeIImm(word); break;

        case 0x63:
            inst.imm = DecodeBImm(word);
            switch (funct3)
            {
            case 0x0: inst.op = Op::BEQ; break;
            case 0x1: inst.op = Op::BNE; break;
            case 0x4: inst.op = Op::BLT; break;
            case 0x5: inst.op = Op::BGE; break;
            case 0x6: inst.op = Op::BLTU; break;
            case 0x7: inst.op = Op::BGEU; break;
            default: inst.op = Op::UNKNOWN; break;
            }
            break;

        case 0x03:
            inst.imm = DecodeIImm(word);
            switch (funct3)
            {
            case 0x0: inst.op = Op::LB; break;
            case 0x1: inst.op = Op::LH; break;
            case 0x2: inst.op = Op::LW; break;
            case 0x4: inst.op = Op::LBU; break;
            case 0x5: inst.op = Op::LHU; break;
            default: inst.op = Op::UNKNOWN; break;
            }
            break;

        case 0x23:
            inst.imm = DecodeSImm(word);
            switch (funct3)
            {
            case 0x0: inst.op = Op::SB; break;
            case 0x1: inst.op = Op::SH; break;
            case 0x2: inst.op = Op::SW; break;
            default: inst.op = Op::UNKNOWN; break;
            }
            break;

        case 0x13:
            switch (funct3)
            {
            case 0x0: inst.op = Op::ADDI; inst.imm = DecodeIImm(word); break;
            case 0x2: inst.op = Op::SLTI; inst.imm = DecodeIImm(word); break;
            case 0x3: inst.op = Op::SLTIU; inst.imm = DecodeIImm(word); break;
            case 0x4: inst.op = Op::XORI; inst.imm = DecodeIImm(word); break;
            case 0x6: inst.op = Op::ORI; inst.imm = DecodeIImm(word); break;
            case 0x7: inst.op = Op::ANDI; inst.imm = DecodeIImm(word); break;
            case 0x1: inst.op = Op::SLLI; inst.imm = static_cast<std::int32_t>(inst.rs2); break;
            case 0x5:
                inst.op = (funct7 == 0x20) ? Op::SRAI : Op::SRLI;
                inst.imm = static_cast<std::int32_t>(inst.rs2); // shamt, not a register index here
                break;
            default: inst.op = Op::UNKNOWN; break;
            }
            break;

        case 0x33:
            switch (funct3)
            {
            case 0x0: inst.op = (funct7 == 0x20) ? Op::SUB : Op::ADD; break;
            case 0x1: inst.op = Op::SLL; break;
            case 0x2: inst.op = Op::SLT; break;
            case 0x3: inst.op = Op::SLTU; break;
            case 0x4: inst.op = Op::XOR; break;
            case 0x5: inst.op = (funct7 == 0x20) ? Op::SRA : Op::SRL; break;
            case 0x6: inst.op = Op::OR; break;
            case 0x7: inst.op = Op::AND; break;
            default: inst.op = Op::UNKNOWN; break;
            }
            break;

        case 0x73:
            if (word == 0x00000073u) inst.op = Op::ECALL;
            else if (word == 0x00100073u) inst.op = Op::EBREAK;
            else inst.op = Op::UNKNOWN;
            break;

        default:
            inst.op = Op::UNKNOWN;
            break;
        }

        return inst;
    }

private:
    static std::uint32_t ToU32(std::bitset<32> bits)
    {
        std::uint32_t value = 0;
        for (int i = 31; i >= 0; --i) value = (value << 1) | (bits[i] ? 1u : 0u);
        return value;
    }

    static std::int32_t SignExtendTo32(std::uint32_t value, std::size_t width)
    {
        return static_cast<std::int32_t>(ToU32(SignExtend::Compute(std::bitset<32>(value), width)));
    }

    static std::int32_t DecodeIImm(std::uint32_t word)
    {
        std::uint32_t imm = (word >> 20) & 0xFFFu;
        return SignExtendTo32(imm, 12);
    }

    static std::int32_t DecodeSImm(std::uint32_t word)
    {
        std::uint32_t imm11_5 = (word >> 25) & 0x7Fu;
        std::uint32_t imm4_0 = (word >> 7) & 0x1Fu;
        std::uint32_t imm = (imm11_5 << 5) | imm4_0;
        return SignExtendTo32(imm, 12);
    }

    static std::int32_t DecodeBImm(std::uint32_t word)
    {
        std::uint32_t imm12 = (word >> 31) & 0x1u;
        std::uint32_t imm11 = (word >> 7) & 0x1u;
        std::uint32_t imm10_5 = (word >> 25) & 0x3Fu;
        std::uint32_t imm4_1 = (word >> 8) & 0xFu;
        std::uint32_t imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);
        return SignExtendTo32(imm, 13);
    }

    static std::int32_t DecodeJImm(std::uint32_t word)
    {
        std::uint32_t imm20 = (word >> 31) & 0x1u;
        std::uint32_t imm19_12 = (word >> 12) & 0xFFu;
        std::uint32_t imm11 = (word >> 20) & 0x1u;
        std::uint32_t imm10_1 = (word >> 21) & 0x3FFu;
        std::uint32_t imm = (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);
        return SignExtendTo32(imm, 21);
    }
};

}

#endif
