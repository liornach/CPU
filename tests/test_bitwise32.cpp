#include "components/bitwise32.hpp"
#include "test_util.hpp"

#include <bitset>
#include <cstdint>

int main()
{
    using ln::Bitwise32;
    using namespace ln::test;

    std::uint32_t values[] = { 0x00000000u, 0xFFFFFFFFu, 0xAAAAAAAAu, 0x55555555u, 0x12345678u, 0xDEADBEEFu };

    for (std::uint32_t a : values)
    {
        Check(Bitwise32::Not(std::bitset<32>(a)) == std::bitset<32>(~a), "NOT mismatch");

        for (std::uint32_t b : values)
        {
            std::bitset<32> ba(a), bb(b);

            Check(Bitwise32::And(ba, bb) == std::bitset<32>(a & b), "AND mismatch");
            Check(Bitwise32::Or(ba, bb) == std::bitset<32>(a | b), "OR mismatch");
            Check(Bitwise32::Xor(ba, bb) == std::bitset<32>(a ^ b), "XOR mismatch");
        }
    }

    return g_failures == 0 ? 0 : 1;
}
