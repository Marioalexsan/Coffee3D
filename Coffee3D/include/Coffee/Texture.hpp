#ifndef COFFEE_TEXTURE_HPP
#define COFFEE_TEXTURE_HPP

#include <GL/glew.h>
#include <span>
#include <Coffee/Color.hpp>
#include <string>
#include <Coffee/PixelFormat.hpp>

namespace coffee
{

    class Texture
    {
    public:

        Texture() = default;
        Texture(const std::string& file);
        Texture(const unsigned char* data, unsigned int width, unsigned int height, PixelFormat format);

        Texture(const Texture&) = delete;
        Texture(Texture&&) noexcept;

        ~Texture();

        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) noexcept;

        bool load(const unsigned char* data, unsigned int width, unsigned int height, PixelFormat format);
        bool load(const std::string& file);

        void unload();

        void bind() const;

    private:

        GLuint		 m_texid = 0;
        unsigned int m_width = 0;
        unsigned int m_height = 0;
    };

}

#endif