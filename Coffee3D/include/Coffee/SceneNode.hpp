#ifndef COFFEE_SCENENODE_HPP
#define COFFEE_SCENENODE_HPP

#include <Coffee/Drawable.hpp>
#include <Coffee/Transformable.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace cf
{
    struct SceneNode : public Transformable
    {
        const Drawable* drawable = nullptr;
    };
}

#endif // COFFEE_SCENENODE_HPP