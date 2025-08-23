
#include "logic_gates.hpp"

namespace CPU
{

class HalfAdder
{
public:
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

    inline void SetInputA(bool a)
    {
        xorGate.SetInputA(a);
        andGate.SetInputA(a);
    }

    inline void SetInputB(bool b)
    {
        xorGate.SetInputB(b);
        andGate.SetInputB(b);
    }

private:
    Gate::AND andGate;
    Gate::XOR xorGate;
};

class FullAdder
{
public:
    inline void SetInputA(bool a)
    {
        if leftHalfAdder.

        leftHalfAdder.SetInputA(a);
        rightHalfAdder.SetInputA(leftHalfAdder.Sum());
        orGate.SetInputB(leftHalfAdder.Carry());
    }

    inline void SetInputB(bool b)
    {
        leftHalfAdder.SetInputB(b);
        rightHalfAdder.SetInputA(leftHalfAdder.Sum());
        orGate.SetInputB(rightHalfAdder.Carry());
    }
private:
    HalfAdder leftHalfAdder, rightHalfAdder;
    Gate::OR orGate;
}

}