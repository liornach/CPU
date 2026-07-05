#include "gates/or_gate.hpp"
#include "test_util.hpp"

#include <bitset>

int main()
{
    using ln::OrGate;
    using namespace ln::test;

    Check(OrGate::Compute(std::bitset<1>(0), std::bitset<1>(0)) == std::bitset<1>(0), "OR(0, 0) == 0");
    Check(OrGate::Compute(std::bitset<1>(0), std::bitset<1>(1)) == std::bitset<1>(1), "OR(0, 1) == 1");
    Check(OrGate::Compute(std::bitset<1>(1), std::bitset<1>(0)) == std::bitset<1>(1), "OR(1, 0) == 1");
    Check(OrGate::Compute(std::bitset<1>(1), std::bitset<1>(1)) == std::bitset<1>(1), "OR(1, 1) == 1");

    return g_failures == 0 ? 0 : 1;
}
