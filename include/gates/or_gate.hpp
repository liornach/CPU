#ifndef LN_GATES_OR_GATE_HPP
#define LN_GATES_OR_GATE_HPP

#include <bitset>

namespace ln
{

struct OrGate
{
    static std::bitset<1> Compute(std::bitset<1> a, std::bitset<1> b)
    {
        return a | b;
    }
};

}

#endif
