#include "components/comparator32.hpp"
#include "test_util.hpp"

#include <bitset>
#include <cstdint>

int main()
{
    using ln::Comparator32;
    using namespace ln::test;

    // --- Signed ---
    Check(Comparator32::LessThanSigned(std::bitset<32>(static_cast<std::uint32_t>(-1)), std::bitset<32>(1u)), "-1 < 1 signed");
    Check(!Comparator32::LessThanSigned(std::bitset<32>(1u), std::bitset<32>(static_cast<std::uint32_t>(-1))), "!(1 < -1) signed");
    Check(!Comparator32::LessThanSigned(std::bitset<32>(5u), std::bitset<32>(5u)), "!(5 < 5) signed");
    Check(Comparator32::LessThanSigned(std::bitset<32>(0x80000000u), std::bitset<32>(1u)), "INT_MIN < 1 signed");
    Check(!Comparator32::LessThanSigned(std::bitset<32>(0x7FFFFFFFu), std::bitset<32>(0x80000000u)), "!(INT_MAX < INT_MIN) signed");

    // --- Unsigned ---
    Check(Comparator32::LessThanUnsigned(std::bitset<32>(1u), std::bitset<32>(2u)), "1 < 2 unsigned");
    Check(!Comparator32::LessThanUnsigned(std::bitset<32>(2u), std::bitset<32>(1u)), "!(2 < 1) unsigned");
    Check(!Comparator32::LessThanUnsigned(std::bitset<32>(5u), std::bitset<32>(5u)), "!(5 < 5) unsigned");
    // 0xFFFFFFFF is the largest unsigned value, so it is never less than anything.
    Check(!Comparator32::LessThanUnsigned(std::bitset<32>(0xFFFFFFFFu), std::bitset<32>(1u)), "!(0xFFFFFFFF < 1) unsigned");
    Check(Comparator32::LessThanUnsigned(std::bitset<32>(1u), std::bitset<32>(0xFFFFFFFFu)), "1 < 0xFFFFFFFF unsigned");

    return g_failures == 0 ? 0 : 1;
}
