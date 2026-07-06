#include "components/full_adder.hpp"
#include "test_util.hpp"

#include <bitset>

int main()
{
    using ln::FullAdder;
    using namespace ln::test;

    struct Case { int a, b, cin, sum, cout; };
    Case cases[] = {
        { 0, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 1, 1, 0, 1 },
        { 1, 0, 0, 1, 0 },
        { 1, 0, 1, 0, 1 },
        { 1, 1, 0, 0, 1 },
        { 1, 1, 1, 1, 1 },
    };

    for (const auto& c : cases)
    {
        auto r = FullAdder::Compute(std::bitset<1>(c.a), std::bitset<1>(c.b), std::bitset<1>(c.cin));
        bool ok = r.sum == std::bitset<1>(c.sum) && r.carry_out == std::bitset<1>(c.cout);
        Check(ok, "full adder case failed");
    }

    return g_failures == 0 ? 0 : 1;
}
