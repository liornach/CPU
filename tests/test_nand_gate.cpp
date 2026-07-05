#include "gates/nand_gate.hpp"

#include <bitset>
#include <iostream>

namespace
{

int g_failures = 0;

void Check(bool condition, const char* description)
{
    if (!condition)
    {
        std::cerr << "FAILED: " << description << '\n';
        ++g_failures;
    }
}

}

int main()
{
    using ln::NandGate;

    Check(NandGate::Compute(std::bitset<1>(0), std::bitset<1>(0)) == std::bitset<1>(1), "NAND(0, 0) == 1");
    Check(NandGate::Compute(std::bitset<1>(0), std::bitset<1>(1)) == std::bitset<1>(1), "NAND(0, 1) == 1");
    Check(NandGate::Compute(std::bitset<1>(1), std::bitset<1>(0)) == std::bitset<1>(1), "NAND(1, 0) == 1");
    Check(NandGate::Compute(std::bitset<1>(1), std::bitset<1>(1)) == std::bitset<1>(0), "NAND(1, 1) == 0");

    return g_failures == 0 ? 0 : 1;
}
