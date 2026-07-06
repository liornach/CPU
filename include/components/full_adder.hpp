#ifndef LN_COMPONENTS_FULL_ADDER_HPP
#define LN_COMPONENTS_FULL_ADDER_HPP

#include "components/half_adder.hpp"
#include "gates/or_gate.hpp"

#include <bitset>

namespace ln
{

struct FullAdder
{
    struct Result
    {
        std::bitset<1> sum;
        std::bitset<1> carry_out;
    };

    static Result Compute(std::bitset<1> a, std::bitset<1> b, std::bitset<1> carry_in)
    {
        HalfAdder::Result first = HalfAdder::Compute(a, b);
        HalfAdder::Result second = HalfAdder::Compute(first.sum, carry_in);
        std::bitset<1> carry_out = OrGate::Compute(first.carry, second.carry);
        return Result{ second.sum, carry_out };
    }
};

}

#endif
