#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <bitset>
#include <stdexcept>
#include <vector>
#include "input.hpp"

namespace CPU
{

template <typename T = std::bitset<1>>
class OutputWire
{
public:
    OutputWire() = default;
    OutputWire(OutputWire& other)
    {
        other.Chain(this);
    }

    void Set(T val)
    {
        _val = val;
        for (auto input : _connectedInputs)
        {
            input->Set(_val);
        }

        if (_connectedOutput)
        {
            _connectedOutput->Set(_val);
        }
    }

    void Chain(InputWire<T>& other)
    {
        _connectedInputs.push_back(&other);
    }

    void Chain(OutputWire& other)
    {
        if (_connectedOutput)
        {
            throw std::logic_error("an output is already connected");
        }
        _connectedOutput = &other;
    }

    T Value() const
    {
        return _val;
    }
private:
    T _val;
    std::vector<InputWire<T>*> _connectedInputs;
    OutputWire *_connectedOutput = NULL;
};

}

#endif