#include "logic_gates.hpp"
#include "tests.hpp"

int main()
{
    auto gate = CPU::OR();
    AssertTrue(gate.SetInputA(true));
    AssertFalse(gate.SetInputA(true));
    AssertTrue(gate.SetInputA(false));
    AssertFalse(gate.SetInputA(false));
    AssertTrue(gate.SetInputA(true));

    return 0;
}