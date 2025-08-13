#ifndef LOGIC_GATES_HPP
#define LOGIC_GATES_HPP

#include "transistor.hpp"

namespace CPU
{

struct OR
{
    OR(const bool& a, const bool& b) : a(a), b(b)
    {
        ;
    }

    inline bool Output()
    {
        return a | b;
    }

private:
    const bool& a, b;
};

struct AND // very interesting... TODO : implement OR
{
    inline bool Output()
    {
        return transistorB.Output();
    }

    inline bool SetInputA(bool a)
    {
        auto wasInputChanged = transistorA.SetInput(a);
        if (!wasInputChanged)
        {
            return Transistor::INPUT_WAS_NOT_CHANGED;
        }

        transistorB.SetCurrent(transistorA.Output());
        return Transistor::INPUT_WAS_CHANGED;
    }

    inline bool SetInputB(bool b)
    {
        return transistorB.SetInput(b);
    }

    inline bool SetCurrent(bool c)
    {   
        auto wasCurrentChanged = transistorA.SetCurrent(c);
        if(!wasCurrentChanged)
        {
            return Transistor::CURRENT_WAS_NOT_CHANGED;
        }

        transistorB.SetCurrent(transistorA.Output());
        return Transistor::CURRENT_WAS_CHANGED;
    }

private:
    Transistor transistorA;
    Transistor transistorB;
};

// nice ,maybe i found a way
struct NOT 
{
    inline bool Output()
    {
        return !transistor.Output();
    }

    inline NOT& SetInput(bool input)
    {
        transistor.SetInput(input);
        return *this;
    }

    inline NOT& SetCurrent(bool c)
    {
        transistor.SetCurrent(c);
        return *this;
    }

private:
    Transistor transistor;
};

// no other choise, work with calbacks
struct XOR
{
    XOR(const bool& a, const bool& b) : notAGate(a), notBGate(b)
    {

    }

    inline bool Output()
    {

    }
private:
    NOT notAGate, notBGate;
    AND aAndNotBGate, bAndNotAGate;
    OR  theTwoAndGates;
}


}

#endif