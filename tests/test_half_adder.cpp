#include "components/half_adder.hpp"
#include "test_util.hpp"

#include <bitset>

int main()
{
    using ln::HalfAdder;
    using namespace ln::test;

    auto r00 = HalfAdder::Compute(std::bitset<1>(0), std::bitset<1>(0));
    Check(r00.sum == std::bitset<1>(0) && r00.carry == std::bitset<1>(0), "0+0 = sum 0 carry 0");

    auto r01 = HalfAdder::Compute(std::bitset<1>(0), std::bitset<1>(1));
    Check(r01.sum == std::bitset<1>(1) && r01.carry == std::bitset<1>(0), "0+1 = sum 1 carry 0");

    auto r10 = HalfAdder::Compute(std::bitset<1>(1), std::bitset<1>(0));
    Check(r10.sum == std::bitset<1>(1) && r10.carry == std::bitset<1>(0), "1+0 = sum 1 carry 0");

    auto r11 = HalfAdder::Compute(std::bitset<1>(1), std::bitset<1>(1));
    Check(r11.sum == std::bitset<1>(0) && r11.carry == std::bitset<1>(1), "1+1 = sum 0 carry 1");

    return g_failures == 0 ? 0 : 1;
}
