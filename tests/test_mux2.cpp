#include "components/mux2.hpp"
#include "test_util.hpp"

#include <bitset>

int main()
{
    using ln::Mux2;
    using namespace ln::test;

    for (int a = 0; a <= 1; ++a)
    {
        for (int b = 0; b <= 1; ++b)
        {
            for (int sel = 0; sel <= 1; ++sel)
            {
                auto result = Mux2::Compute(std::bitset<1>(a), std::bitset<1>(b), std::bitset<1>(sel));
                int expected = sel == 0 ? a : b;
                Check(result == std::bitset<1>(expected), "mux2 case failed");
            }
        }
    }

    return g_failures == 0 ? 0 : 1;
}
