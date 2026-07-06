#ifndef LN_COMPONENTS_HALF_ADDER_HPP
#define LN_COMPONENTS_HALF_ADDER_HPP

#include "gates/and_gate.hpp"
#include "gates/xor_gate.hpp"

#include <bitset>

namespace ln
{

struct HalfAdder
{
    struct Result
    {
        std::bitset<1> sum;
        std::bitset<1> carry;
    };

    static Result Compute(std::bitset<1> a, std::bitset<1> b)
    {
        return Result{ XorGate::Compute(a, b), AndGate::Compute(a, b) };
    }
};

}

#endif
