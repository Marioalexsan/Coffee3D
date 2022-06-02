#ifndef COFFEE_MODEL_HPP
#define COFFEE_MODEL_HPP

#include <GL/glew.h>
#include <Coffee/Vertex.hpp>
#include <glm/glm.hpp>

#include <span>

namespace cf
{
	class Model
	{
		friend class Object3D;
		friend class Skybox;

	public:

		Model() = default;
		Model(const Model&) = delete;
		Model(Model&&) noexcept;

		~Model();

		Model& operator=(const Model&) = delete;
		Model& operator=(Model&&) noexcept;

		bool load(const std::span<Vertex>& vertices, const std::span<glm::uvec3>& order = std::span<glm::uvec3>());
		void unload();

	private:
		std::size_t m_triangleCount = 0;
		GLuint m_vaoid = 0;
		GLuint m_vboid = 0;
		GLuint m_eboid = 0;
	};
}

#endif // COFFEE_MODEL_HPP