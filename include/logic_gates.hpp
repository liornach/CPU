#ifndef LOGIC_GATES_HPP
#define LOGIC_GATES_HPP


namespace CPU::Gate
{

struct DualGate
{
    virtual bool Output() const = 0;
    virtual void SetInputA(bool a)
    {
        ia = a;
    }

    virtual void SetInputB(bool b)
    {
        ib = b;
    }

    virtual ~DualGate() = default;
protected:
    bool ia = false;
    bool ib = false;
};

struct OR : DualGate
{
    bool Output() const override
    {
        return ia || ib;
    }
};

struct AND : DualGate
{
    bool Output() const override
    {
        return ia && ib;
    }
};

struct NOT 
{
    bool Output() const
    {
        return !i;
    }

    void SetInput(bool input)
    {
        i = input;
    }

private:
    bool i = false;
};


struct XOR
{
    bool Output() const
    {
        return aOrBandNotAandB.Output();
    }

    void SetInputA(bool a)
    {
        aOrB.SetInputA(a);
        aAndB.SetInputA(a);
        notAandB.SetInput(aAndB.Output());
        aOrBandNotAandB.SetInputA(aOrB.Output());
        aOrBandNotAandB.SetInputB(notAandB.Output());        
    }

    void SetInputB(bool b)
    {
        aOrB.SetInputB(b);
        aAndB.SetInputB(b);
        notAandB.SetInput(aAndB.Output());
        aOrBandNotAandB.SetInputA(aOrB.Output());
        aOrBandNotAandB.SetInputB(notAandB.Output());
    }

private:
    OR aOrB;
    AND aAndB;
    NOT notAandB;
    AND aOrBandNotAandB;
};

}

#endif