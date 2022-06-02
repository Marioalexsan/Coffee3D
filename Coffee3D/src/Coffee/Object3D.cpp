#include <Coffee/Object3D.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cf
{
	void Object3D::model(const Model* model)
	{
		m_model = model;
	}

	const Model* Object3D::model()
	{
		return m_model;
	}

	void Object3D::texture(const Texture* texture)
	{
		m_texture = texture;
	}

	const Texture* Object3D::texture()
	{
		return m_texture;
	}

	void Object3D::draw(RenderState state) const
	{
		if (!(m_model && m_model->m_vaoid))
			return;

		glBindVertexArray(m_model->m_vaoid);

		state.modelMatrix *= getMatrix();

		glm::mat4 mvpMatrix = state.projectionMatrix * state.viewMatrix * state.modelMatrix;

		if (m_texture)
			state.texture = m_texture;

		if (state.texture)
			state.texture->bind();

		if (state.shader)
		{
			state.shader->bind();

			GLuint matrixID = glGetUniformLocation(state.shader->programId(), "mvpMatrix");
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

			GLuint texID = glGetUniformLocation(state.shader->programId(), "tex");
			glUniform1i(texID, 0);
		}

		// Draw with elements
		if (m_model->m_eboid)
		{
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_model->m_triangleCount) * 3, GL_UNSIGNED_INT, (void*)0);
		}

		// Regular draw
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_model->m_triangleCount) * 3);
		}
	}
}