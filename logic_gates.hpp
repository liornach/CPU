#ifndef LOGIC_GATES_HPP
#define LOGIC_GATES_HPP

#include "cassert" // assert
#include "transistor.hpp" // Transistor
#include <cassert>

namespace CPU
{

struct OR
{
    inline bool Output()
    {
        if (!ta.Output())
        {
            return tb.Output();
        }

        return true;
    }

    inline bool SetInputA(bool a)
    {
        return ta.SetInput(a);
    }

    inline bool SetInputB(bool b)
    {
        return tb.SetInput(b);
    }

    inline bool SetCurrent(bool c)
    {
        auto changed = ta.SetCurrent(c);
        if (!changed)
        {
            return changed;
        }

        changed = tb.SetCurrent(c);
        assert(changed);
        return changed;
    }

private:
    Transistor ta;
    Transistor tb;
};

struct AND
{
    inline bool Output()
    {
        return tb.Output();
    }

    inline bool SetInputA(bool a)
    {
        auto changed = ta.SetInput(a);
        if (!changed)
        {
            return Transistor::INPUT_WAS_NOT_CHANGED;
        }

        tb.SetCurrent(ta.Output());
        return Transistor::INPUT_WAS_CHANGED;
    }

    inline bool SetInputB(bool b)
    {
        return tb.SetInput(b);
    }

    inline bool SetCurrent(bool c)
    {   
        auto changed = ta.SetCurrent(c);
        if(!changed)
        {
            return Transistor::CURRENT_WAS_NOT_CHANGED;
        }

        tb.SetCurrent(ta.Output());
        return Transistor::CURRENT_WAS_CHANGED;
    }

private:
    Transistor ta;
    Transistor tb;
};

struct NOT 
{
    inline bool Output()
    {
        return !transistor.Output();
    }

    inline bool SetInput(bool input)
    {
        return transistor.SetInput(input);
    }

    inline bool SetCurrent(bool c)
    {
        return transistor.SetCurrent(c);
    }

private:
    Transistor transistor;
};


}

#endif