#ifndef COFFEE_VERTEX_HPP
#define COFFEE_VERTEX_HPP

#include <glm/glm.hpp>
#include <Coffee/Color.hpp>

namespace coffee
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texCoords;
		Color color;

		Vertex() :
			position(),
			texCoords(),
			color()
		{
		}

		Vertex(glm::vec3 position, glm::vec2 texCoords, Color color) :
			position(position),
			texCoords(texCoords),
			color(color)
		{
		}
	};
}

#endif