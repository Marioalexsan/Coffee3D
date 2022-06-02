#include <Coffee/CubemapTexture.hpp>
#include <memory>

#include <stb/stb_image.h>
#include <GL/glew.h>
#include <map>
#include <Coffee/PixelFormat.hpp>

namespace
{
    const std::map<cf::PixelFormat, GLint> formatMap = {
        {cf::PixelFormat::RGB, GL_RGB},
        {cf::PixelFormat::RGBA, GL_RGBA}
    };
}

namespace cf
{
    CubemapTexture::CubemapTexture(const std::array<std::string, 6>& faces)
    {
        load(faces);
    }

    CubemapTexture::CubemapTexture(const std::array<const unsigned char*, 6>& faces, unsigned int width, unsigned int height, PixelFormat format)
    {
        load(faces, width, height, format);
    }

    CubemapTexture::CubemapTexture(CubemapTexture&& other) noexcept
    {
        *this = std::move(other);
    }

    CubemapTexture::~CubemapTexture()
    {
        unload();
    }

    CubemapTexture& CubemapTexture::operator=(CubemapTexture&& other) noexcept
    {
        m_texid = other.m_texid;

        other.m_texid = 0;

        return *this;
    }

    bool CubemapTexture::load(const std::array<const unsigned char*, 6>& faces, unsigned int width, unsigned int height, PixelFormat format)
    {
        if (!formatMap.contains(format))
            return false;

        GLint pixelFormat = formatMap.at(format);

        if (!m_texid)
        {
            glGenTextures(1, &m_texid);
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texid);

        for (int i = 0; i < 6; i++)
        {
            // Faces go one after another
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, faces[i]);
        }

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return true;
    }

    bool CubemapTexture::load(const std::array<std::string, 6>& faces)
    {
        int width = 0, height = 0, nrChannels = 0;

        std::array<const unsigned char*, 6> data;

        for (int i = 0; i < 6; i++)
        {
            data[i] = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 4);
        }

        bool status = load(data, width, height, PixelFormat::RGBA);

        for (int i = 0; i < 6; i++)
        {
            stbi_image_free((void*)data[i]);
        }

        return status;
    }

    void CubemapTexture::unload()
    {
        if (m_texid)
        {
            glDeleteTextures(1, &m_texid);
            m_texid = 0;
        }
    }

    void CubemapTexture::bind() const
    {
        if (m_texid)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_texid);
        }
    }
}