#ifndef INPUT_HPP
#define INPUT_HPP

#include <bitset>
#include <cassert>
#include <functional>
#include <vector>

namespace CPU
{

template <typename T = std::bitset<1>>
class InputWire
{
public:
    void Set(T val)
    {
        _val = val;
        for (const auto& change : _onChange)
        {
            change(_val);
        }
    }

    void OnChange(std::function<void(T)> onChange)
    {
        _onChange.push_back(onChange);
    }

    void Chain(InputWire& other)
    {
        _onChange.push_back([&](T v){other.Set(v);});
    }

    operator std::function<void(T)>()
    {
        return [&](T val){this->Set(val);};
    }

    T Value() const
    {
        return _val;
    }

private:
    T _val;
    std::vector<std::function<void(T)>> _onChange;
};

}

#endif