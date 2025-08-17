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
        if (!ta.Ground())
        {
            return tb.Ground();
        }

        return true;
    }

    inline bool SetInputA(bool a)
    {
        return ta.SetControlGate(a);
    }

    inline bool SetInputB(bool b)
    {
        return tb.SetControlGate(b);
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
        return tb.Ground();
    }

    inline bool SetInputA(bool a)
    {
        if (!ta.SetControlGate(a))
        {
            return false;
        }

        tb.SetCurrent(ta.Ground());
        return true;
    }

    inline bool SetInputB(bool b)
    {
        return tb.SetControlGate(b);
    }

    inline bool SetCurrent(bool c)
    {   
        if(!ta.SetCurrent(c))
        {
            return false;
        }

        tb.SetCurrent(ta.Ground());
        return true;
    }

private:
    Transistor ta;
    Transistor tb;
};

struct NOT 
{
    inline bool Output()
    {
        if (!transistor.Ground())
        {
            return false;
        }
        
        return true;
    }

    inline bool SetInput(bool input)
    {
        return transistor.SetControlGate(input);
    }

    inline bool SetCurrent(bool c)
    {
        return transistor.SetCurrent(c);
    }

private:
    Transistor transistor;
};

struct AOI
{
    inline bool SetInputA(bool val)
    {
        if (!andGate.SetInputA(val))
        {
            return false;
        }
        
        orGate.SetInputA(andGate.Output());
        return true;
    }

    inline bool SetInputB(bool val)
    {
        if (!andGate.SetInputB(val))
        {
            return false;
        }
        
        orGate.SetInputA(andGate.Output());
        return true;
    }

    inline bool SetInputC(bool val)
    {
        return orGate.SetInputB(val);
    } 

    inline bool Output()
    {
        return orGate.Output();
    }
private:
    OR orGate;
    AND andGate;
};

struct XOR
{
    inline bool SetInputA(bool a)
    {
        bool orGateChange = orGate.SetInputA(a);
        if (orGateChange)
        {
            aoiGate.SetInputA(orGate.Output());
        }

        bool aoiGateChange = aoiGate.SetInputB(a);
        if (!aoiGateChange && !orGateChange)
        {
            return false;
        }

        return true;
    }

private:
    OR orGate;
    AOI aoiGate;
};

}

#endif