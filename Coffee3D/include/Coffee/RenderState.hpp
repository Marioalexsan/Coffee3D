#ifndef COFFEE_RENDERSTATE_HPP
#define COFFEE_RENDERSTATE_HPP

#include <Coffee/Texture.hpp>
#include <Coffee/Shader.hpp>
#include <glm/glm.hpp>

namespace coffee
{
    struct RenderState
    {
        const Texture* texture = nullptr;
        const Shader* shader = nullptr;
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
    };
}

#endif