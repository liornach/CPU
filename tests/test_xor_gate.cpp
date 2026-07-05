#include "gates/xor_gate.hpp"
#include "test_util.hpp"

#include <bitset>

int main()
{
    using ln::XorGate;
    using namespace ln::test;

    Check(XorGate::Compute(std::bitset<1>(0), std::bitset<1>(0)) == std::bitset<1>(0), "XOR(0, 0) == 0");
    Check(XorGate::Compute(std::bitset<1>(0), std::bitset<1>(1)) == std::bitset<1>(1), "XOR(0, 1) == 1");
    Check(XorGate::Compute(std::bitset<1>(1), std::bitset<1>(0)) == std::bitset<1>(1), "XOR(1, 0) == 1");
    Check(XorGate::Compute(std::bitset<1>(1), std::bitset<1>(1)) == std::bitset<1>(0), "XOR(1, 1) == 0");

    return g_failures == 0 ? 0 : 1;
}
