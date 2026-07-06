#ifndef LN_COMPONENTS_MUX2_HPP
#define LN_COMPONENTS_MUX2_HPP

#include "gates/and_gate.hpp"
#include "gates/not_gate.hpp"
#include "gates/or_gate.hpp"

#include <bitset>

namespace ln
{

struct Mux2
{
    // select == 0 -> a, select == 1 -> b
    static std::bitset<1> Compute(std::bitset<1> a, std::bitset<1> b, std::bitset<1> select)
    {
        std::bitset<1> notSelect = NotGate::Compute(select);
        std::bitset<1> left = AndGate::Compute(a, notSelect);
        std::bitset<1> right = AndGate::Compute(b, select);
        return OrGate::Compute(left, right);
    }
};

}

#endif
