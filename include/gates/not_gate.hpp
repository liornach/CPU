#ifndef LN_GATES_NOT_GATE_HPP
#define LN_GATES_NOT_GATE_HPP

#include <bitset>

namespace ln
{

struct NotGate
{
    static std::bitset<1> Compute(std::bitset<1> a)
    {
        return ~a;
    }
};

}

#endif
