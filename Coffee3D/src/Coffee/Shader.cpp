#include <Coffee/Shader.hpp>
#include <memory>
#include <map>

namespace
{
    const std::string fs_code =
        "#version 400\n"

        "in vec3 Position;"
        "in vec2 TexCoord;"
        "in vec4 Color;"

        "out vec4 FragColor;"

        "uniform sampler2D tex;"

        "void main()"
        "{"
        "FragColor = Color * texture(tex, TexCoord);"
        "}";

    const std::string vs_code =
        "#version 400\n"

        "layout(location = 0) in vec3 aPos;"
        "layout(location = 1) in vec2 aTexCoord;"
        "layout(location = 2) in vec4 aColor;"

        "out vec3 Position;"
        "out vec2 TexCoord;"
        "out vec4 Color;"

        "uniform sampler2D tex;"
        "uniform mat4x4 mvpMatrix;"

        "void main()"
        "{"
        "gl_Position = mvpMatrix * vec4(aPos, 1.0);"
        "Color = aColor;"
        "TexCoord = aTexCoord;"
        "Position = aPos;"
        "}";

    const std::map<cf::ShaderType, GLenum> shaderTypeMap = {
        {cf::ShaderType::Vertex, GL_VERTEX_SHADER},
        {cf::ShaderType::Fragment, GL_FRAGMENT_SHADER}
    };

    const std::map<cf::ShaderType, std::string> shaderDisplay = {
        {cf::ShaderType::Vertex, "Vertex Shader"},
        {cf::ShaderType::Fragment, "Fragment Shader"}
    };
}

namespace cf
{
    Shader::Shader(Shader&& other) noexcept
    {
        *this = std::move(other);
    }

    Shader::~Shader()
    {
        destroy();
    }

    Shader& Shader::operator=(Shader&& other) noexcept
    {
        for (int i = 0; i < 2; i++)
        {
            this->m_shaders[i] = other.m_shaders[i];
            other.m_shaders[i] = 0;
        }

        this->m_program = other.m_program;
        other.m_program = 0;

        return *this;
    }

    bool Shader::load(ShaderType type, const std::string& source)
    {
        if (!shaderTypeMap.contains(type))
            return false;

        std::size_t index = static_cast<std::size_t>(type);

        if (!m_shaders[index])
            m_shaders[index] = glCreateShader(shaderTypeMap.at(type));

        const char* data = source.data();
        const int length = static_cast<const int>(source.size());

        glShaderSource(m_shaders[index], 1, &data, &length);

        return m_status = compile();
    }

    bool Shader::compile()
    {
        if (!m_program)
            m_program = glCreateProgram();

        for (int i = 0; i < 2; i++)
        {
            if (m_shaders[i])
            {
                glCompileShader(m_shaders[i]);
                glAttachShader(m_program, m_shaders[i]);
            }
        }

        glLinkProgram(m_program);

        GLint status = 0;

        for (int i = 0; i < 2; i++)
        {
            if (m_shaders[i])
                glGetShaderiv(m_shaders[i], GL_COMPILE_STATUS, &status);

            if (status != GL_TRUE)
                return false;
        }

        glGetProgramiv(m_program, GL_LINK_STATUS, &status);

        return status == GL_TRUE;
    }

    void Shader::destroy()
    {
        for (int i = 0; i < 2; i++)
        {
            if (m_shaders[i])
            {
                if (m_program)
                    glDetachShader(m_program, m_shaders[i]);

                glDeleteShader(m_shaders[i]);
            }
        }

        if (m_program)
            glDeleteProgram(m_program);
    }

    void Shader::bind() const
    {
        if (m_program)
            glUseProgram(m_program);
    }

    bool Shader::ready() const
    {
        return m_status;
    }

    std::string Shader::getShaderLog(ShaderType type) const
    {
        std::size_t index = static_cast<std::size_t>(type);

        if (!shaderTypeMap.contains(type) || !m_shaders[index])
            return "Not created.";

        int length = 0, written = 0;
        std::string log;

        glGetShaderiv(m_shaders[index], GL_INFO_LOG_LENGTH, &length);

        std::string data = std::string(length, ' ');

        if (length > 0)
            glGetShaderInfoLog(m_shaders[index], length, &written, data.data());

        return data;
    }

    std::string Shader::getProgramLog() const
    {
        if (!m_program)
            return "Not created.";

        int length = 0, written = 0;
        std::string log;

        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);

        if (length > 0)
        {
            std::string data = std::string(length, ' ');

            glGetProgramInfoLog(m_program, length, &written, data.data());

            log += "Program error log: \n" + data;
        }

        return log;
    }

    std::string Shader::getCombinedLogs() const
    {
        std::string log;

        log += "Vertex Shader Log:\n";
        log += getShaderLog(ShaderType::Vertex);
        log += "Fragment Shader Log:\n";
        log += getShaderLog(ShaderType::Fragment);
        log += "Program Log:\n";
        log += getProgramLog();

        return log;
    }

    GLuint Shader::programId() const
    {
        return m_program;
    }

    Shader Shader::defaultShader()
    {
        auto shader = Shader();
        shader.load(ShaderType::Vertex, vs_code);
        shader.load(ShaderType::Fragment, fs_code);
        return shader;
    }
}