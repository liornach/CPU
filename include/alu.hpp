#ifndef ALU_HPP
#define ALU_HPP

#include "input.hpp"
#include "output.hpp"
namespace CPU
{



class ALU 
{
public:
    
    InputWire<> OpCode, InputA, InputB;
    OutputWire Sum, Carry;
};

}

#endif