#ifndef LOGIC_GATES_HPP
#define LOGIC_GATES_HPP

#include <bitset>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <algorithm>
namespace ln
{

typedef std::bitset<1> input_t;
typedef std::bitset<1> output_t;


class Output
{
public:
    void Set(output_t val)
    {

    }
};

class Input
{
public:
    void Set(input_t val)
    {
        _input = val;
        for (const auto i : _connectedInputs)
        {
            i->Set(val);
        }
    }

    static void Connect(Input& left, Input& right)
    {
        if (AreConnected(left, right))
        {
            throw std::runtime_error("inputs are already connected");
        }

        left._connectedInputs.push_back(&right);
        right._inputsToDisconnectFromWhenDestoryed.push_back(&left);
    }

    static bool AreConnected(const Input& left, const Input& right)
    {
        const auto& linputs = left._connectedInputs;
        const auto& rinputs = right._connectedInputs;

        return 
            std::find(linputs.begin(), linputs.end(), &right) != linputs.end() ||
            std::find(rinputs.begin(), rinputs.end(), &left) != rinputs.end()  ;
    }

    static void Disconnect(Input& it, Input& from)
    {

    }

    
    ~Input()
    {
        
    }
    
protected:
    static std::vector<Input*>::iterator Find(Input* item)

    input_t _input;
    std::vector<Input*> _connectedInputs;
    std::vector<Input*> _inputsToDisconnectFromWhenDestoryed;

private:
    const std::vector<Input*>& connectedInputs()
    {
        return _connectedInputs;
    }

    bool isConnectedTo(const Input& other)
    {
        bool isThisOneConnectedToOther =
            std::find(other._connectedInputs.begin(), other._connectedInputs.end(), this) != other._connectedInputs.end();
        
        if (isThisOneConnectedToOther)
        {
            return true;
        }

        bool isOtherConnectedToThis =
            std::find(_connectedInputs.begin(), _connectedInputs.end(), &other) != _connectedInputs.end();

        return isOtherConnectedToThis;
    }
};

class IGate
{
public:
    inline void SetInputA(input_t a)
    {
        m_inputA = a;
        m_output = UpdateOutput();
    }

    virtual output_t Output() const = 0;
    virtual void ConnectOutputTo(IGate& other);
    virtual void ConnectInputAToOtherGateOutput(IGate& other)
    {
        
    }

protected:
    output_t UpdateOutput();

    input_t m_inputA;
    output_t m_output;
};

class IDualGate : public IGate
{
public:
    virtual void SetInputB(input_t b) = 0;
    virtual void ConnectInputBTo(IGate& other);

protected:
    input_t m_inputB;
};


class OrGate : public IDualGate
{
public:
    virtual output_t Output()
    {

    }

    virtual void ConnectOutputTo(IGate& other) override
    {

    }

    virtual void ConnectInputATo(IGate& other) override
    {

    }

    virtual void ConnectInputBTo(IGate& other) override
    {

    }    
protected:
    
    inline output_t UpdateOutput()
    {
        return m_inputA | m_inputB;
    }
};

}


#endif