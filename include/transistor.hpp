#ifndef TRANSISTOR_HPP
#define TRANSISTOR_HPP

namespace CPU
{

struct Transistor
{
    static const bool CONTROL_GATE_WAS_CHANGED = true;
    static const bool CONTROL_GATE_WAS_NOT_CHANGED = false;
    static const bool CURRENT_WAS_CHANGED = true;
    static const bool CURRENT_WAS_NOT_CHANGED = false;

    inline bool Ground() const
    {
        if (!current)
        {
            return false;
        }

        return ctrlGate;
    }

    inline bool SetControlGate(bool val)
    {
        if (ctrlGate == val)
        {
            return CONTROL_GATE_WAS_NOT_CHANGED;
        }

        ctrlGate = val;
        return CONTROL_GATE_WAS_CHANGED;
    }

    inline bool SetCurrent(bool c)
    {
        if (c == current)
        {
            return CURRENT_WAS_NOT_CHANGED;
        }


        current = c;
        return CURRENT_WAS_CHANGED;
    }

private:
    bool ctrlGate : 1 = false;
    bool current : 1 = false;
};

}

#endif