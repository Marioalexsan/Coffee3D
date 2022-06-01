#ifndef COFFEE_SHADER_HPP
#define COFFEE_SHADER_HPP

#include <GL/glew.h>
#include <string>
#include <Coffee/ShaderType.hpp>

namespace cf
{
    class Shader
    {
    public:
        
        Shader() = default;
        Shader(const Shader&) = delete;
        Shader(Shader&&) noexcept;

        ~Shader();

        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) noexcept;

        bool load(ShaderType type, const std::string& source);

        void destroy();

        void bind() const;

        bool ready() const;

        std::string getShaderLog(ShaderType type) const;
        std::string getProgramLog() const;

        std::string getCombinedLogs() const;

        GLuint programId() const;

    private:
        bool compile();

        bool m_status = false;
        GLuint m_shaders[2];
        GLuint m_program;
    };
}

#endif