#ifndef LN_COMPONENTS_BITWISE32_HPP
#define LN_COMPONENTS_BITWISE32_HPP

#include "gates/and_gate.hpp"
#include "gates/not_gate.hpp"
#include "gates/or_gate.hpp"
#include "gates/xor_gate.hpp"

#include <bitset>
#include <cstddef>

namespace ln
{

// 32-bit bitwise operations, each built by fanning a 1-bit gate out across all 32 bit positions.
struct Bitwise32
{
    static std::bitset<32> And(std::bitset<32> a, std::bitset<32> b) { return Apply(a, b, AndGate::Compute); }
    static std::bitset<32> Or(std::bitset<32> a, std::bitset<32> b) { return Apply(a, b, OrGate::Compute); }
    static std::bitset<32> Xor(std::bitset<32> a, std::bitset<32> b) { return Apply(a, b, XorGate::Compute); }

    static std::bitset<32> Not(std::bitset<32> a)
    {
        std::bitset<32> result;
        for (std::size_t i = 0; i < 32; ++i)
        {
            result[i] = NotGate::Compute(std::bitset<1>(a[i]))[0];
        }
        return result;
    }

private:
    template <typename Gate>
    static std::bitset<32> Apply(std::bitset<32> a, std::bitset<32> b, Gate gate)
    {
        std::bitset<32> result;
        for (std::size_t i = 0; i < 32; ++i)
        {
            result[i] = gate(std::bitset<1>(a[i]), std::bitset<1>(b[i]))[0];
        }
        return result;
    }
};

}

#endif
