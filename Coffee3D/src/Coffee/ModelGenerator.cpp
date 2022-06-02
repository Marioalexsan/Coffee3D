#include <Coffee/ModelGenerator.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <map>
#include <Coffee/Utility.hpp>

namespace
{
	
}

namespace cf
{
	Model ModelGenerator::sphere()
	{
		constexpr float pidouble = 2.f * glm::pi<float>();
		constexpr float pi = glm::pi<float>();
		constexpr float pihalf = glm::pi<float>();

		// Note: We need to duplicate vertices along seams
		// so that the texture mapping works correctly.

		std::vector<Vertex> vertices;
		std::vector<glm::uvec3> triangles;

		const int detail = 20;

		const int rows = detail;
		const int cols = detail;

		for (int row = 0; row < rows; row++)
		{
			for (int col = 0; col < cols; col++)
			{
				float aFactor = glm::pi<float>() * row / (rows - 1);
				float bFactor = 2 * glm::pi<float>() * col / (cols - 1);

				float z = std::sin(aFactor) * std::cos(bFactor);
				float x = std::sin(aFactor) * std::sin(bFactor);
				float y = std::cos(aFactor);

				float sx = bFactor / pidouble;
				float sy = aFactor / pi;

				sx = cf::clamp(sx, 0.f, 1.f);
				sy = cf::clamp(sy, 0.f, 1.f);

				vertices.emplace_back(
					glm::vec3(x, y, z),
					glm::vec2(sx, sy),
					Color::White
					);
			}
		}

		// Create triangles for the vertex matrix

		for (int row = 0; row < rows - 1; row++)
		{
			for (int col = 0; col < cols - 1; col++)
			{
				int topLeft = row * cols + col;
				int topRight = row * cols + col + 1;
				int bottomLeft = (row + 1) * cols + col;
				int bottomRight = (row + 1) * cols + col + 1;

				triangles.push_back(glm::uvec3(topLeft, bottomLeft, topRight));
				triangles.push_back(glm::uvec3(bottomRight, topRight, bottomLeft));
			}
		}

		Model model;

		model.load(vertices, triangles);

		return model;
	}

	Model ModelGenerator::box()
	{
		std::vector<Vertex> vertices = {
			// Bottom face
			{ glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0), Color::White },
			{ glm::vec3(+0.5, -0.5, -0.5), glm::vec2(0, 1), Color::White },
			{ glm::vec3(-0.5, -0.5, +0.5), glm::vec2(1, 0), Color::White },
			{ glm::vec3(+0.5, -0.5, +0.5), glm::vec2(1, 1), Color::White },

			// Front face
			{ glm::vec3(-0.5, -0.5, +0.5), glm::vec2(0, 0), Color::White },
			{ glm::vec3(+0.5, -0.5, +0.5), glm::vec2(0, 1), Color::White },
			{ glm::vec3(-0.5, +0.5, +0.5), glm::vec2(1, 0), Color::White },
			{ glm::vec3(+0.5, +0.5, +0.5), glm::vec2(1, 1), Color::White },

			// Left face
			{ glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0), Color::White },
			{ glm::vec3(-0.5, -0.5, +0.5), glm::vec2(0, 1), Color::White },
			{ glm::vec3(-0.5, +0.5, -0.5), glm::vec2(1, 0), Color::White },
			{ glm::vec3(-0.5, +0.5, +0.5), glm::vec2(1, 1), Color::White },

			// Back face
			{ glm::vec3(+0.5, -0.5, -0.5), glm::vec2(0, 0), Color::White },
			{ glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 1), Color::White },
			{ glm::vec3(+0.5, +0.5, -0.5), glm::vec2(1, 0), Color::White },
			{ glm::vec3(-0.5, +0.5, -0.5), glm::vec2(1, 1), Color::White },

			// Right face
			{ glm::vec3(+0.5, -0.5, +0.5), glm::vec2(0, 0), Color::White },
			{ glm::vec3(+0.5, -0.5, -0.5), glm::vec2(0, 1), Color::White },
			{ glm::vec3(+0.5, +0.5, +0.5), glm::vec2(1, 0), Color::White },
			{ glm::vec3(+0.5, +0.5, -0.5), glm::vec2(1, 1), Color::White },

			// Top face
			{ glm::vec3(-0.5, +0.5, +0.5), glm::vec2(0, 0), Color::White },
			{ glm::vec3(+0.5, +0.5, +0.5), glm::vec2(0, 1), Color::White },
			{ glm::vec3(-0.5, +0.5, -0.5), glm::vec2(1, 0), Color::White },
			{ glm::vec3(+0.5, +0.5, -0.5), glm::vec2(1, 1), Color::White }
		};

		std::vector<glm::uvec3> order = {
			// Bottom face
			glm::uvec3(0, 1, 2),
			glm::uvec3(1, 2, 3),

			// Front face
			glm::uvec3(4, 5, 6),
			glm::uvec3(5, 6, 7),

			// Left face
			glm::uvec3(8, 9, 10),
			glm::uvec3(9, 10, 11),

			// Back face
			glm::uvec3(12, 13, 14),
			glm::uvec3(13, 14, 15),

			// Right face
			glm::uvec3(16, 17, 18),
			glm::uvec3(17, 18, 19),

			// Top face
			glm::uvec3(20, 21, 22),
			glm::uvec3(21, 22, 23)
		};

		Model model;

		model.load(vertices, order);

		return model;
	}
}