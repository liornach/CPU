#ifndef RIPPLE_CARRY_ADDER_HPP
#define RIPPLE_CARRY_ADDER_HPP

#include "adders.hpp"
#include "input.hpp"
#include <bitset>
#include <cstddef>


namespace CPU
{

template<std::size_t Bits>
class RippleCarryAdder
{
    RippleCarryAdder()
    {
        InputA.OnChange([&](auto bits){ // fix this shit
            std::bitset<1> carry = 0;
            for (size_t i = 0; i < bits.size(); ++i)
            {
                auto& adder = _fullAdders[i];
                adder.InputA = 
            }
        ;});
    }

    InputWire<std::bitset<Bits>> InputA;
    InputWire<std::bitset<Bits>> InputB;
private:
    FullAdder _fullAdders[Bits];
};

}

#endif