#ifndef INPUT_HPP
#define INPUT_HPP

#include <cassert>
#include <functional>
#include <vector>

namespace CPU
{

class InputWire
{
public:
    void Set(bool val)
    {
        _val = val;
        for (const auto& change : _onChange)
        {
            change(_val);
        }
    }

    void Connect(std::function<void(bool)> onChange)
    {
        _onChange.push_back(onChange);
    }

    void Connect(InputWire& other)
    {
        _onChange.push_back([&](bool v){other.Set(v);});
    }

    operator std::function<void(bool)>()
    {
        return [&](bool val){this->Set(val);};
    }

    bool Value() const
    {
        return _val;
    }

private:
    bool _val = false;
    std::vector<std::function<void(bool)>> _onChange;
};

}

#endif