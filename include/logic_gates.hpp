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
    virtual ~Gate() = default;
    InputWire<> InputA;
    OutputWire<> Output;
};

struct DualGate : public Gate
{
    virtual ~DualGate() = default;
    InputWire<> InputB;
};

struct TripletGate : DualGate
{
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

struct AoiGate : public TripletGate
{
    AoiGate()
    {

    }
private:

};

struct XorGate : public DualGate
{
    XorGate()
    {
        aAndBGate.Output.Chain(notAandBGate.InputA);
        aOrBGate.Output.Chain(aOrBandNotAandB.InputA);
        notAandBGate.Output.Chain(aOrBandNotAandB.InputB);

        this->InputA.Chain(aOrBGate.InputA);
        this->InputA.Chain(aAndBGate.InputA);
        this->InputB.Chain(aOrBGate.InputB);
        this->InputB.Chain(aAndBGate.InputB);

        aOrBandNotAandB.Output.Chain(Output);
    }

private:
    OrGate aOrBGate;
    AndGate aAndBGate;
    NotGate notAandBGate;
    AndGate aOrBandNotAandB;
};

}

#endif