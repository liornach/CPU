#include "gates/nand_gate.hpp"
#include "test_util.hpp"

#include <bitset>

int main()
{
    using ln::NandGate;
    using namespace ln::test;

    Check(NandGate::Compute(std::bitset<1>(0), std::bitset<1>(0)) == std::bitset<1>(1), "NAND(0, 0) == 1");
    Check(NandGate::Compute(std::bitset<1>(0), std::bitset<1>(1)) == std::bitset<1>(1), "NAND(0, 1) == 1");
    Check(NandGate::Compute(std::bitset<1>(1), std::bitset<1>(0)) == std::bitset<1>(1), "NAND(1, 0) == 1");
    Check(NandGate::Compute(std::bitset<1>(1), std::bitset<1>(1)) == std::bitset<1>(0), "NAND(1, 1) == 0");

    return g_failures == 0 ? 0 : 1;
}
