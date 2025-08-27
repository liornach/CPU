#ifndef LOGIC_GATES_HPP
#define LOGIC_GATES_HPP

#include <cassert>
#include <functional>
#include "input.hpp"
#include "output.hpp"

namespace CPU::Gate
{

struct Gate
{
    Gate() = default;
    // Gate(OutputWire<>& output) : Output(output) {}
    // Gate(InputWire<>& input) : InputA(input) {}
    // Gate(OutputWire<>& output, InputWire<>& input) : InputA(input), Output(output) {}

    virtual ~Gate() = default;

    InputWire<> InputA;
    OutputWire<> Output;
};

struct DualGate : public Gate
{
    DualGate() = default;
    // DualGate(OutputWire<>& output) : Gate(output) {}
    // DualGate(InputWire<>& a, InputWire<>& b) : Gate(a), InputB(b) {}
    // DualGate(OutputWire<>& output, InputWire<>& a, InputWire<>& b) : Gate(output, a), InputB(b) {}

    virtual ~DualGate() = default;

    InputWire<> InputB;
};

struct TripletGate : DualGate
{
    TripletGate() = default;
    // TripletGate(OutputWire<>& output) : DualGate(output) {}
    // TripletGate(InputWire<>& a, InputWire<>& b, InputWire<>& c) : DualGate(a, b), InputC(c) {}
    //TripletGate(OutputWire<>& output, InputWire<>& a, InputWire<>& b, InputWire<>& c) : DualGate(output, a, b), InputC(c) {}
    
    virtual ~TripletGate() = default;

    InputWire<>InputC;
};

struct OrGate : public DualGate
{
    OrGate()
    {
        InputA.OnChange([&](auto v){Output.Set(v | InputB.Value());});
        InputB.OnChange([&](auto v){Output.Set(v | InputA.Value());});
    }
};

struct AndGate : public DualGate
{
    AndGate()
    {
        InputA.OnChange([&](auto v){Output.Set(v & InputB.Value());});
        InputB.OnChange([&](auto v){Output.Set(v & InputA.Value());});
    }
};

struct NotGate : public Gate
{
    NotGate()
    {
        InputA.OnChange([&](auto v){Output.Set(~v);});
    }
};

struct NorGate : public DualGate
{
    NorGate()
    {
        InputA.Chain(_or.InputA);
        InputB.Chain(_or.InputB);

        _or.Output.Chain(_not.InputA);
        _not.Output.Chain(this->Output);
    }
private:
    OrGate _or;
    NotGate _not;
};

struct AoiGate : public TripletGate
{
    AoiGate()
    {
        InputA.Chain(_and.InputA);
        InputB.Chain(_and.InputB);
        
        _and.Output.Chain(_nor.InputA);
        InputC.Chain(_nor.InputB);
        
        _nor.Output.Chain(this->Output);
    }
private:
    NorGate _nor;
    AndGate _and;
};


struct XorGate : public DualGate
{
    XorGate()
    {
        InputA.Chain(_nor.InputA);
        InputA.Chain(_aoi.InputB);

        InputB.Chain(_nor.InputB);
        InputB.Chain(_aoi.InputC);

        _nor.Output.Chain(_aoi.InputA);

        _aoi.Output.Chain(this->Output);
    }

private:
    AoiGate _aoi;
    NorGate _nor;
};

}

#endif