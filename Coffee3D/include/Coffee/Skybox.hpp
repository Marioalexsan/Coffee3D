#ifndef COFFEE_SKYBOX_HPP
#define COFFEE_SKYBOX_HPP

#include <Coffee/Drawable.hpp>
#include <Coffee/CubemapTexture.hpp>
#include <Coffee/Model.hpp>

namespace cf
{
    class Skybox : public Drawable
    {
    public:
        Skybox();
        Skybox(const CubemapTexture* texture);

        void texture(const CubemapTexture* texture);
        const CubemapTexture* texture();

        void draw(RenderState state) const override;

    private:
        static Shader s_skyboxShader;

        Model m_box;
        const CubemapTexture* m_texture;
    };
}

#endif // COFFEE_SKYBOX_HPP