#include "logic_gates.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


struct Entry
{
    bool A, B;
};

int main()
{
    std::vector<Entry> vec;
    vec.push_back({true, true});
    vec.push_back({true, false});
    vec.push_back({false, false});
    vec.push_back({false, true});
    CPU::Gate::XorGate xorGate;
    auto tests = 0;

    for (const auto& e : vec)
    {
        auto a = e.A;
        auto b = e.B;
        bool exp = a ^ b;
        xorGate.InputA.Set(a);
        xorGate.InputB.Set(b);
        auto res = xorGate.Output();
        if (res != exp)
        {
            std::string err = "expected : " + std::to_string(exp) + ", result : " + std::to_string(res) + ", input a : " + std::to_string(a) + ", input b : " + std::to_string(b);
            throw std::logic_error(err);
        }

        ++tests;
    }

    std::cout << "passed " << tests << " tests" << std::endl;

    return 0;
}