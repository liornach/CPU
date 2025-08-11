#ifndef OR_GATE_HPP
#define OR_GATE_HPP

namespace gates
{

struct GateInputs
{
    inline bool A() { return a; }

    inline bool B() { return b; }

    inline void SetA(bool v) { a = v; }

    inline void SetB(bool v) { b = v; }

private:
    bool a, b;
};

class IGate
{
public:
    bool Output();
    GateInputs &Inputs();
};

class OrGate : public IGate
{
public:
    inline OrGate() : inputs(GateInputs()) {}

    inline bool Output() { return inputs.A() | inputs.B(); }

    inline GateInputs &Inputs() { return inputs; }

private:
    GateInputs inputs;
};

class AndGate : public IGate
{
public:
    inline AndGate() : inputs(GateInputs()) {}

    inline bool Output() { return inputs.A() & inputs.B(); }

    inline GateInputs &Inputs() { return inputs; }

private:
    GateInputs inputs;
};
} // namespace gates
#endif
