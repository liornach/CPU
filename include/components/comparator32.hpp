#ifndef LN_COMPONENTS_COMPARATOR32_HPP
#define LN_COMPONENTS_COMPARATOR32_HPP

#include "components/adder32.hpp"

#include <bitset>

namespace ln
{

// Signed/unsigned "less than" built on top of the adder/subtractor's a - b path:
// unsigned:  a < b  <=>  the subtraction borrowed  <=>  carry_out == 0
// signed:    a < b  <=>  sign(a - b) XOR overflow(a - b)
struct Comparator32
{
    static bool LessThanUnsigned(std::bitset<32> a, std::bitset<32> b)
    {
        Adder32::Result r = Adder32::Compute(a, b, true);
        return !r.carry_out;
    }

    static bool LessThanSigned(std::bitset<32> a, std::bitset<32> b)
    {
        Adder32::Result r = Adder32::Compute(a, b, true);
        bool resultSign = r.value[31];
        return resultSign != r.overflow;
    }
};

}

#endif
