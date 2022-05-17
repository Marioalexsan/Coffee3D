#ifndef COFFEE_SCENE_HPP
#define COFFEE_SCENE_HPP

#include <Coffee/Drawable.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace coffee
{
    class SceneNode
    {
    public:
        SceneNode() = default;

        SceneNode(const Drawable* drawable) :
            m_drawable(drawable)
        { }

        void renderScene(RenderState state) const;

        const Drawable*         m_drawable = nullptr;
        std::vector<SceneNode>  m_children;
        glm::mat4				m_transform = glm::mat4(1.0f);

    private:

        void renderSceneDFS(RenderState state) const;
        void renderSceneBFS(RenderState state) const;
    };
}

#endif // COFFEE_SCENE_HPP