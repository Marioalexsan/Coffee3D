#ifndef COFFEE_MODELGENERATOR_HPP
#define COFFEE_MODELGENERATOR_HPP

#include <Coffee/Model.hpp>

namespace cf
{
    struct ModelGenerator
    {
        ModelGenerator() = delete;

		static Model sphere();

        static Model box();
    };
}

#endif