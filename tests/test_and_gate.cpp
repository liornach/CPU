#include "gates/and_gate.hpp"
#include "test_util.hpp"

#include <bitset>

int main()
{
    using ln::AndGate;
    using namespace ln::test;

    Check(AndGate::Compute(std::bitset<1>(0), std::bitset<1>(0)) == std::bitset<1>(0), "AND(0, 0) == 0");
    Check(AndGate::Compute(std::bitset<1>(0), std::bitset<1>(1)) == std::bitset<1>(0), "AND(0, 1) == 0");
    Check(AndGate::Compute(std::bitset<1>(1), std::bitset<1>(0)) == std::bitset<1>(0), "AND(1, 0) == 0");
    Check(AndGate::Compute(std::bitset<1>(1), std::bitset<1>(1)) == std::bitset<1>(1), "AND(1, 1) == 1");

    return g_failures == 0 ? 0 : 1;
}
