#include "gates/nor_gate.hpp"
#include "test_util.hpp"

#include <bitset>

int main()
{
    using ln::NorGate;
    using namespace ln::test;

    Check(NorGate::Compute(std::bitset<1>(0), std::bitset<1>(0)) == std::bitset<1>(1), "NOR(0, 0) == 1");
    Check(NorGate::Compute(std::bitset<1>(0), std::bitset<1>(1)) == std::bitset<1>(0), "NOR(0, 1) == 0");
    Check(NorGate::Compute(std::bitset<1>(1), std::bitset<1>(0)) == std::bitset<1>(0), "NOR(1, 0) == 0");
    Check(NorGate::Compute(std::bitset<1>(1), std::bitset<1>(1)) == std::bitset<1>(0), "NOR(1, 1) == 0");

    return g_failures == 0 ? 0 : 1;
}
