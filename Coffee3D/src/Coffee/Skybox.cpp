#include <Coffee/Skybox.hpp>
#include <Coffee/ModelGenerator.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace
{
	const std::string fs_code =
		"#version 400\n"

		"in vec3 Position;"
		"in vec3 TexCoord;"
		"in vec4 Color;"

		"out vec4 FragColor;"

		"uniform samplerCube tex;"

		"void main()"
		"{"
		"FragColor = texture(tex, TexCoord);"
		"}";

	const std::string vs_code =
		"#version 400\n"

		"layout(location = 0) in vec3 aPos;"
		"layout(location = 1) in vec2 aTexCoord;"
		"layout(location = 2) in vec4 aColor;"

		"out vec3 Position;"
		"out vec3 TexCoord;"
		"out vec4 Color;"

		"uniform mat4x4 mvpMatrix;"

		"void main()"
		"{"
		"vec4 pos = mvpMatrix * vec4(aPos, 1.0);"
		"gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);"
		"Color = aColor;"
		"TexCoord = vec3(aPos.x, aPos.y, -aPos.z);"
		"Position = aPos;"
		"}";
}

namespace cf
{
	Shader Skybox::s_skyboxShader = Shader();

    Skybox::Skybox() :
        m_texture(nullptr),
        m_box(ModelGenerator::box())
    {
		if (!s_skyboxShader.ready())
		{
			s_skyboxShader.load(ShaderType::Vertex, vs_code);
			s_skyboxShader.load(ShaderType::Fragment, fs_code);

			std::cout << "Skybox Shader load status: " << (s_skyboxShader.ready() ? "Ready" : "Not ready") << "\n";
			std::cout << "Skybox Shader logs: " << s_skyboxShader.getCombinedLogs() << "\n";
		}
    }

    Skybox::Skybox(const CubemapTexture* tex) :
        m_texture(tex),
        m_box(ModelGenerator::box())
    {
		if (!s_skyboxShader.ready())
		{
			s_skyboxShader.load(ShaderType::Vertex, vs_code);
			s_skyboxShader.load(ShaderType::Fragment, fs_code);

			std::cout << "Skybox Shader load status: " << (s_skyboxShader.ready() ? "Ready" : "Not ready") << "\n";
			std::cout << "Skybox Shader logs: " << s_skyboxShader.getCombinedLogs() << "\n";
		}
    }

    void Skybox::texture(const CubemapTexture* texture)
    {
        m_texture = texture;
    }

    const CubemapTexture* Skybox::texture()
    {
        return m_texture;
    }

    void Skybox::draw(RenderState state) const
    {
		if (!m_box.m_vaoid)
			return;

		glBindVertexArray(m_box.m_vaoid);

		state.modelMatrix = glm::mat4(1.0f);
		state.viewMatrix = glm::mat4(glm::mat3(state.viewMatrix));

		glm::mat4 mvpMatrix = state.projectionMatrix * state.viewMatrix * state.modelMatrix;

		m_texture->bind();

		s_skyboxShader.bind();

		GLuint matrixID = glGetUniformLocation(state.shader->programId(), "mvpMatrix");
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

		GLuint texID = glGetUniformLocation(state.shader->programId(), "tex");
		glUniform1i(texID, 0);

		glDepthFunc(GL_LEQUAL);

		// Draw with elements
		if (m_box.m_eboid)
		{
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_box.m_triangleCount) * 3, GL_UNSIGNED_INT, (void*)0);
		}

		// Regular draw
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_box.m_triangleCount) * 3);
		}

		glDepthFunc(GL_LESS);


		if (state.shader)
		{
			state.shader->bind();
		}
    }
}