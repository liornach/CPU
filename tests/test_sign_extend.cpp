#include "components/sign_extend.hpp"
#include "test_util.hpp"

#include <bitset>
#include <cstdint>

int main()
{
    using ln::SignExtend;
    using namespace ln::test;

    // 8-bit positive (0x7F) extended to 32 bits stays 0x0000007F.
    Check(SignExtend::Compute(std::bitset<32>(0x7Fu), 8) == std::bitset<32>(0x0000007Fu), "8-bit positive sign-extend");

    // 8-bit negative (0x80 == -128 in 8 bits) extended to 32 bits becomes 0xFFFFFF80.
    Check(SignExtend::Compute(std::bitset<32>(0x80u), 8) == std::bitset<32>(0xFFFFFF80u), "8-bit negative sign-extend");

    // 12-bit negative (RV32I I-immediate width): 0xFFF (-1 in 12 bits) -> 0xFFFFFFFF.
    Check(SignExtend::Compute(std::bitset<32>(0xFFFu), 12) == std::bitset<32>(0xFFFFFFFFu), "12-bit negative sign-extend");

    // 12-bit positive: 0x7FF -> 0x000007FF.
    Check(SignExtend::Compute(std::bitset<32>(0x7FFu), 12) == std::bitset<32>(0x000007FFu), "12-bit positive sign-extend");

    // Already 32 bits: sign-extending from 32 is a no-op.
    Check(SignExtend::Compute(std::bitset<32>(0xDEADBEEFu), 32) == std::bitset<32>(0xDEADBEEFu), "32-bit no-op");

    return g_failures == 0 ? 0 : 1;
}
