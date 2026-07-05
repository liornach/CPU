#include "gates/not_gate.hpp"
#include "test_util.hpp"

#include <bitset>

int main()
{
    using ln::NotGate;
    using namespace ln::test;

    Check(NotGate::Compute(std::bitset<1>(0)) == std::bitset<1>(1), "NOT(0) == 1");
    Check(NotGate::Compute(std::bitset<1>(1)) == std::bitset<1>(0), "NOT(1) == 0");

    return g_failures == 0 ? 0 : 1;
}
