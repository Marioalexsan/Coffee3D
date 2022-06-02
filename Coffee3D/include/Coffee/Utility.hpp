#ifndef COFFEE_UTILITY_HPP
#define COFFEE_UTILITY_HPP

#include <functional>

namespace cf
{
    template <class T>
    T clamp(T value, T min, T max)
    {
        if (value > max)
            value = max;
        else if (value < min)
            value = min;

        return value;
    }
}

#endif // COFFEE_UTILITY_HPP