#include <Coffee/Model.hpp>
#include <GL/glew.h>

namespace
{
}

namespace cf
{
	Model::~Model()
	{
		unload();
	}

	Model::Model(Model&& other) noexcept
	{
		*this = std::move(other);
	}

	Model& Model::operator=(Model&& other) noexcept
	{
		this->m_eboid = other.m_eboid;
		this->m_vboid = other.m_vboid;
		this->m_vaoid = other.m_vaoid;
		this->m_triangleCount = other.m_triangleCount;

		other.m_eboid = 0;
		other.m_vboid = 0;
		other.m_vaoid = 0;
		other.m_triangleCount = 0;

		return *this;
	}

	bool Model::load(const std::span<Vertex>& vertices, const std::span<glm::uvec3>& order)
	{
		// Always generate the VAO and bind it
		// Otherwise we may mess up the previously bound VAO (if any)
		if (!m_vaoid)
		{
			glGenVertexArrays(1, &m_vaoid);
		}

		glBindVertexArray(m_vaoid);

		if (!m_vboid)
		{
			glCreateBuffers(1, &m_vboid);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vboid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		// Use element draw
		if (order.size() != 0)
		{
			if (!m_eboid)
			{
				glCreateBuffers(1, &m_eboid);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboid);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * order.size(), order.data(), GL_STATIC_DRAW);

			m_triangleCount = order.size();
		}

		// Clear the element buffer if present
		else if (m_eboid)
		{
			glDeleteBuffers(1, &m_eboid);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vboid);

		if (m_eboid)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboid);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, sizeof(Vertex::position) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, sizeof(Vertex::texCoords) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::position)));
		glVertexAttribPointer(2, sizeof(Vertex::color) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::position) + sizeof(Vertex::texCoords)));

		return true;
	}

	void Model::unload()
	{
		if (m_vaoid)
		{
			glDeleteVertexArrays(1, &m_vaoid);
			m_vaoid = 0;
		}

		if (m_vboid)
		{
			glDeleteBuffers(1, &m_vboid);
			m_vboid = 0;
		}

		if (m_eboid)
		{
			glDeleteBuffers(1, &m_eboid);
			m_eboid = 0;
		}
	}
}