#ifndef ADDERS_HPP
#define ADDERS_HPP


#include "input.hpp"
#include "logic_gates.hpp"
#include <cassert>
#include <functional>

namespace CPU
{

class HalfAdder
{
public:

    HalfAdder()
    {
        InputA.Connect(xorGate.InputA);
        InputA.Connect(andGate.InputA);
    }

    inline void SetInputA(bool a)
    {
        xorGate.InputA.Set(a);
        andGate.InputA.Set(a);
        callConnected();
    }

    inline void SetInputB(bool b)
    {
        xorGate.InputB.Set(b);
        andGate.InputB.Set(b);
        callConnected();
    }

    struct Output
    {
        bool Sum, Carry;
    };

    inline bool Carry() const
    {
        return andGate.Output();
    }

    inline bool Sum() const
    {
        return xorGate.Output();
    }

    inline void ConnectCarry(std::function<void(bool)> onInputSet)
    {
        assert(_connectedToCarry == NULL);
        _connectedToCarry = onInputSet;
    }

    inline void ConnectSum(std::function<void(bool)> onInputSet)
    {
        assert(_connectedToSum == NULL);
        _connectedToSum = onInputSet;
    }



    InputWire InputA; 
    InputWire InputB;

private:
    void callConnected() const
    {
        if (_connectedToSum != NULL)
        {
            _connectedToSum(Sum());
        }

        if (_connectedToCarry != NULL)
        {
            _connectedToCarry(Carry());
        }
    }

    std::function<void(bool)> _connectedToCarry;
    std::function<void(bool)> _connectedToSum;
    Gate::AndGate andGate;
    Gate::XorGate xorGate;
    bool _sum = 0, _carry = 0;
};

class FullAdder
{
public:
    FullAdder()
    {
        firstHalfAdder.ConnectSum(secondHalfAdder.InputA);
        firstHalfAdder.ConnectCarry(orGate.InputA);
        secondHalfAdder.ConnectCarry(orGate.InputB);
    }

    inline bool Sum() const
    {
        return secondHalfAdder.Sum();
    }

    inline bool Carry() const
    {
        return orGate.Output();
    }

    inline void SetInputA(bool a)
    {
        firstHalfAdder.SetInputA(a);
    }

    inline void SetInputB(bool b)
    {
        firstHalfAdder.SetInputB(b);
    }

    inline void SetInputC(bool c)
    {
        secondHalfAdder.SetInputB(c);
    }
private:
    HalfAdder firstHalfAdder, secondHalfAdder;
    Gate::OrGate orGate;
};

}

#endif