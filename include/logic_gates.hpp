#ifndef LOGIC_GATES_HPP
#define LOGIC_GATES_HPP


#include <cassert>
#include <functional>
#include "input.hpp"

namespace CPU::Gate
{

struct Gate
{
    Gate()
    {
        InputA.Connect([&](bool _){OnChange();});
    }

    virtual ~Gate() = default;
    InputWire InputA;
    virtual bool Output() const = 0;
    
    virtual void ConnectToOutput(InputWire& input)
    {
        assert(_connectedInput == NULL);
        _connectedInput = &input;
    }
    
    inline void OnChange()
    {
        if (!_connectedInput)
        {
            return;
        }

        _connectedInput->Set(Output());
    }

protected:
    InputWire *_connectedInput = NULL;
};

struct DualGate : public Gate
{
    DualGate()
    {
        InputB.Connect([&](bool _){OnChange();});
    }
    virtual ~DualGate() = default;
    InputWire InputB;
};

struct OrGate : public DualGate
{
    bool Output() const override
    {
        return InputA.Value() || InputB.Value();
    }
};

struct AndGate : public DualGate
{
    bool Output() const override
    {
        return InputA.Value() && InputB.Value();
    }
};

struct NotGate : public Gate
{
    bool Output() const
    {
        return !InputA.Value();
    }
};


struct XorGate : public DualGate
{
    XorGate()
    {
        aAndBGate.ConnectToOutput(notAandBGate.InputA);
        aOrBGate.ConnectToOutput(aOrBandNotAandB.InputA);
        notAandBGate.ConnectToOutput(aOrBandNotAandB.InputB);
        this->InputA.Connect(aOrBGate.InputA);
        //InputA.Connect([&](bool v){aOrBGate.InputA.Set(v);});
        //InputA.Connect([&](bool v){aAndBGate.InputA.Set(v);});
        this->InputA.Connect(aAndBGate.InputA);
        //InputB.Connect([&](bool v){aOrBGate.InputB.Set(v);});
        //InputB.Connect([&](bool v){aAndBGate.InputB.Set(v);});
        this->InputB.Connect(aOrBGate.InputB);
        this->InputB.Connect(aAndBGate.InputB);
    }

    bool Output() const
    {
        return aOrBandNotAandB.Output();
    }

private:
    OrGate aOrBGate;
    AndGate aAndBGate;
    NotGate notAandBGate;
    AndGate aOrBandNotAandB;
};

}

#endif