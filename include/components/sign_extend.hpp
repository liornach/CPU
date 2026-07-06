#ifndef LN_COMPONENTS_SIGN_EXTEND_HPP
#define LN_COMPONENTS_SIGN_EXTEND_HPP

#include <bitset>
#include <cstddef>

namespace ln
{

// Sign-extends the low `bits` bits of `value` to fill all 32 bits.
// This is bit replication, not boolean logic, so it's implemented directly
// rather than composed from gates.
struct SignExtend
{
    static std::bitset<32> Compute(std::bitset<32> value, std::size_t bits)
    {
        std::bitset<32> result = value;
        bool signBit = value[bits - 1];
        for (std::size_t i = bits; i < 32; ++i)
        {
            result[i] = signBit;
        }
        return result;
    }
};

}

#endif
