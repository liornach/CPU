#ifndef TRANSISTOR_HPP
#define TRANSISTOR_HPP

namespace CPU
{

struct Transistor
{
    static const bool INPUT_WAS_CHANGED = true;
    static const bool INPUT_WAS_NOT_CHANGED = false;
    static const bool CURRENT_WAS_CHANGED = true;
    static const bool CURRENT_WAS_NOT_CHANGED = false;

    inline bool Output() const
    {
        return current && input;
    }

    inline bool SetInput(bool i)
    {
        if (input == i)
        {
            return INPUT_WAS_NOT_CHANGED;
        }

        input = i;
        return INPUT_WAS_CHANGED;
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
    bool input : 1 = false;
    bool current : 1 = false;
};

}

#endif