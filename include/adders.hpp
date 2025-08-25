#ifndef ADDERS_HPP
#define ADDERS_HPP

#include "input.hpp"
#include "logic_gates.hpp"
#include "output.hpp"

namespace CPU
{

class HalfAdder
{
public:

    HalfAdder()
    {
        this->InputA.Chain(this->aXorB.InputA);
        this->InputB.Chain(this->aXorB.InputB);

        this->InputA.Chain(this->aAndB.InputA);
        this->InputB.Chain(this->aAndB.InputB);

        this->aXorB.Output.Chain(this->Sum);
        this->aAndB.Output.Chain(this->Carry);
    }

    InputWire<> InputA; 
    InputWire<> InputB;
    OutputWire<> Carry;
    OutputWire<> Sum;

private:
    Gate::AndGate aAndB;
    Gate::XorGate aXorB;
};

class FullAdder
{
public:
    FullAdder()
    {
        InputA.Chain(firstHalfAdder.InputA);
        InputB.Chain(firstHalfAdder.InputB);
        InputC.Chain(secondHalfAdder.InputB);

        firstHalfAdder.Sum.Chain(secondHalfAdder.InputA);
        firstHalfAdder.Carry.Chain(carryOrGate.InputA);
        secondHalfAdder.Carry.Chain(carryOrGate.InputB);

        secondHalfAdder.Sum.Chain(Sum);
        carryOrGate.Output.Chain(Carry);
    }

    InputWire<> InputA, InputB, InputC;
    OutputWire<> Sum, Carry;
private:
    HalfAdder firstHalfAdder, secondHalfAdder;
    Gate::OrGate carryOrGate;
};

}

#endif