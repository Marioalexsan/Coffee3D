#ifndef COFFEE_MODELGENERATOR_HPP
#define COFFEE_MODELGENERATOR_HPP

#include <Coffee/Model.hpp>

namespace coffee
{
    struct ModelGenerator
    {
        ModelGenerator() = delete;

		static Model sphere();
    };
}

#endif