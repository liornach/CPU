#include "gates/nand_gate.hpp"

#include <cassert>
#include <bitset>

int main()
{
    using ln::NandGate;

    assert(NandGate::Compute(std::bitset<1>(0), std::bitset<1>(0)) == std::bitset<1>(1));
    assert(NandGate::Compute(std::bitset<1>(0), std::bitset<1>(1)) == std::bitset<1>(1));
    assert(NandGate::Compute(std::bitset<1>(1), std::bitset<1>(0)) == std::bitset<1>(1));
    assert(NandGate::Compute(std::bitset<1>(1), std::bitset<1>(1)) == std::bitset<1>(0));

    return 0;
}
