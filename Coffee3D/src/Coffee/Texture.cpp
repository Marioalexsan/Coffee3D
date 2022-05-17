#include <Coffee/Texture.hpp>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <GL/glew.h>
#include <map>
#include <Coffee/PixelFormat.hpp>

namespace
{
    const std::map<coffee::PixelFormat, GLint> formatMap = {
        {coffee::PixelFormat::RGB, GL_RGB},
        {coffee::PixelFormat::RGBA, GL_RGBA}
    };
}

namespace coffee
{
    Texture::Texture(const std::string& file)
    {
        load(file);
    }

    Texture::Texture(const unsigned char* data, unsigned int width, unsigned int height, PixelFormat format)
    {
        load(data, width, height, format);
    }

    Texture::Texture(Texture&& other) noexcept
    {
        *this = std::move(other);
    }

    Texture::~Texture()
    {
        unload();
    }

    Texture& Texture::operator=(Texture&& other) noexcept
    {
        m_texid = other.m_texid;
        m_width = other.m_width;
        m_height = other.m_height;

        other.m_texid = 0;
        other.m_width = 0;
        other.m_height = 0;

        return *this;
    }

    bool Texture::load(const unsigned char* data, unsigned int width, unsigned int height, PixelFormat format)
    {
        if (!formatMap.contains(format))
            return false;

        m_width = width;
        m_height = height;

        GLint pixelFormat = formatMap.at(format);

        if (!m_texid)
        {
            glGenTextures(1, &m_texid);
        }

        glBindTexture(GL_TEXTURE_2D, m_texid);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return true;
    }

    bool Texture::load(const std::string& filePath)
    {
        int width = 0, height = 0, nrChannels = 0;

        std::unique_ptr<uint8_t> data(stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0));

        if (nrChannels == 3)
            return load(data.get(), width, height, PixelFormat::RGB);

        else if (nrChannels == 4)
            return load(data.get(), width, height, PixelFormat::RGBA);

        else return false;
    }

    void Texture::unload()
    {
        if (m_texid)
        {
            glDeleteTextures(1, &m_texid);
            m_texid = 0;
        }
    }

    void Texture::bind() const
    {
        if (m_texid)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texid);
        }
    }
}