#ifndef COFFEE_CUBEMAPTEXTURE_HPP
#define COFFEE_CUBEMAPTEXTURE_HPP

#include <array>
#include <string>
#include <Coffee/PixelFormat.hpp>
#include <GL/glew.h>

namespace cf
{
    class CubemapTexture
    {
    public:

        CubemapTexture() = default;
        CubemapTexture(const std::array<std::string, 6>& faces);
        CubemapTexture(const std::array<const unsigned char*, 6>& faces, unsigned int width, unsigned int height, PixelFormat format);

        CubemapTexture(const CubemapTexture&) = delete;
        CubemapTexture(CubemapTexture&&) noexcept;

        ~CubemapTexture();

        CubemapTexture& operator=(const CubemapTexture&) = delete;
        CubemapTexture& operator=(CubemapTexture&&) noexcept;

        bool load(const std::array<const unsigned char*, 6>& faces, unsigned int width, unsigned int height, PixelFormat format);
        bool load(const std::array<std::string, 6>& faces);

        void unload();

        void bind() const;

    private:

        GLuint		 m_texid = 0;
    };
}

#endif // COFFEE_CUBEMAPTEXTURE_HPP