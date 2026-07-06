#include "components/adder32.hpp"
#include "test_util.hpp"

#include <bitset>
#include <cstdint>

int main()
{
    using ln::Adder32;
    using namespace ln::test;

    // --- Addition ---
    {
        auto r = Adder32::Compute(std::bitset<32>(0u), std::bitset<32>(0u), false);
        Check(r.value == std::bitset<32>(0u) && !r.carry_out && !r.overflow, "0 + 0 == 0");
    }
    {
        auto r = Adder32::Compute(std::bitset<32>(2u), std::bitset<32>(3u), false);
        Check(r.value == std::bitset<32>(5u) && !r.carry_out && !r.overflow, "2 + 3 == 5");
    }
    {
        // 0xFFFFFFFF + 1 wraps to 0 with carry out, no *signed* overflow (-1 + 1 == 0).
        auto r = Adder32::Compute(std::bitset<32>(0xFFFFFFFFu), std::bitset<32>(1u), false);
        Check(r.value == std::bitset<32>(0u) && r.carry_out && !r.overflow, "0xFFFFFFFF + 1 wraps with carry, no signed overflow");
    }
    {
        // INT_MAX + 1 signed-overflows into INT_MIN.
        auto r = Adder32::Compute(std::bitset<32>(0x7FFFFFFFu), std::bitset<32>(1u), false);
        Check(r.value == std::bitset<32>(0x80000000u) && r.overflow, "INT_MAX + 1 signed-overflows");
    }

    // --- Subtraction ---
    {
        auto r = Adder32::Compute(std::bitset<32>(5u), std::bitset<32>(3u), true);
        Check(r.value == std::bitset<32>(2u) && !r.overflow, "5 - 3 == 2");
    }
    {
        auto r = Adder32::Compute(std::bitset<32>(3u), std::bitset<32>(5u), true);
        Check(r.value == std::bitset<32>(static_cast<std::uint32_t>(-2)) && !r.overflow, "3 - 5 == -2");
    }
    {
        // INT_MIN - 1 signed-overflows (can't represent -2147483649).
        auto r = Adder32::Compute(std::bitset<32>(0x80000000u), std::bitset<32>(1u), true);
        Check(r.value == std::bitset<32>(0x7FFFFFFFu) && r.overflow, "INT_MIN - 1 signed-overflows");
    }
    {
        // Unsigned: 1 - 2 borrows, so carry_out == 0.
        auto r = Adder32::Compute(std::bitset<32>(1u), std::bitset<32>(2u), true);
        Check(!r.carry_out, "1u - 2u borrows (carry_out clear)");
    }
    {
        // Unsigned: 5 - 3 does not borrow, so carry_out == 1.
        auto r = Adder32::Compute(std::bitset<32>(5u), std::bitset<32>(3u), true);
        Check(r.carry_out, "5u - 3u does not borrow (carry_out set)");
    }

    return g_failures == 0 ? 0 : 1;
}
