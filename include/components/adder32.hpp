#ifndef LN_COMPONENTS_ADDER32_HPP
#define LN_COMPONENTS_ADDER32_HPP

#include "components/full_adder.hpp"
#include "components/mux2.hpp"
#include "gates/not_gate.hpp"

#include <bitset>
#include <cstddef>

namespace ln
{

// 32-bit ripple-carry adder/subtractor. Subtraction reuses the same adder chain by
// feeding each B bit through a mux selecting B or ~B (controlled by `subtract`), with
// carry-in seeded to 1 - the standard two's-complement add/sub trick, so no separate
// subtractor circuit is needed.
struct Adder32
{
    struct Result
    {
        std::bitset<32> value;
        bool carry_out;
        bool overflow; // signed overflow of the operation actually performed (a + (subtract ? ~b : b) + subtract)
    };

    static Result Compute(std::bitset<32> a, std::bitset<32> b, bool subtract)
    {
        std::bitset<32> result;
        std::bitset<1> subtractBit(subtract ? 1u : 0u);
        std::bitset<1> carry(subtract ? 1u : 0u);

        bool effectiveBMsb = false;

        for (std::size_t i = 0; i < 32; ++i)
        {
            std::bitset<1> ai(a[i]);
            std::bitset<1> bi_raw(b[i]);
            std::bitset<1> bi_inverted = NotGate::Compute(bi_raw);
            std::bitset<1> bi = Mux2::Compute(bi_raw, bi_inverted, subtractBit);

            FullAdder::Result sum = FullAdder::Compute(ai, bi, carry);
            result[i] = sum.sum[0];
            carry = sum.carry_out;

            if (i == 31) effectiveBMsb = bi[0];
        }

        Result out;
        out.value = result;
        out.carry_out = carry[0];

        bool msbA = a[31];
        bool msbResult = result[31];
        out.overflow = (msbA == effectiveBMsb) && (msbResult != msbA);

        return out;
    }
};

}

#endif
