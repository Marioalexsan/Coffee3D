#include <Coffee/ModelGenerator.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <map>

namespace
{
	struct Edge
	{
		int v0;
		int v1;

		Edge(int v0, int v1)
			: v0(v0 < v1 ? v0 : v1)
			, v1(v0 < v1 ? v1 : v0)
		{
		}

		bool operator<(const Edge& edge) const
		{
			return v0 < edge.v0 || (v0 == edge.v0 && v1 < edge.v1);
		}
	};

	static int subdivideEdge(int f0, int f1, const glm::vec3& v0, const glm::vec3& v1,
		std::vector<glm::vec3>& vertices, std::map<Edge, int>& divisions)
	{
		Edge edge(f0, f1);
		auto it = divisions.find(edge);
		if (it != divisions.end()) return it->second;
		glm::vec3 v = glm::normalize(glm::vec3(0.5) * (v0 + v1));
		int f = (int)vertices.size();
		vertices.push_back(v);
		divisions[edge] = f;
		return f;
	}

	static void subdivideMesh(
		std::vector<glm::vec3> verticesIn, std::vector<glm::uvec3> trianglesIn,
		std::vector<glm::vec3>& verticesOut, std::vector<glm::uvec3>& trianglesOut)
	{
		verticesOut = verticesIn;
		std::map<Edge, int> divisions; // Edge -> new vertex
		for (int i = 0; i < trianglesIn.size(); ++i)
		{
			int f0 = trianglesIn[i][0];
			int f1 = trianglesIn[i][1];
			int f2 = trianglesIn[i][2];
			glm::vec3 v0 = verticesIn[f0];
			glm::vec3 v1 = verticesIn[f1];
			glm::vec3 v2 = verticesIn[f2];
			int f3 = subdivideEdge(f0, f1, v0, v1, verticesOut, divisions);
			int f4 = subdivideEdge(f1, f2, v1, v2, verticesOut, divisions);
			int f5 = subdivideEdge(f2, f0, v2, v0, verticesOut, divisions);
			trianglesOut.push_back(glm::uvec3(f0, f3, f5));
			trianglesOut.push_back(glm::uvec3(f3, f1, f4));
			trianglesOut.push_back(glm::uvec3(f4, f2, f5));
			trianglesOut.push_back(glm::uvec3(f3, f4, f5));
		}
	}
}

namespace coffee
{
	Model ModelGenerator::sphere()
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::uvec3> triangles;

		const float phi = 1.618033f; // golden ratio

		vertices.push_back(glm::normalize(glm::vec3(-1.0, phi, 0.0)));
		vertices.push_back(glm::normalize(glm::vec3(1.0, phi, 0.0)));
		vertices.push_back(glm::normalize(glm::vec3(-1.0, -phi, 0.0)));
		vertices.push_back(glm::normalize(glm::vec3(1.0, -phi, 0.0)));
		vertices.push_back(glm::normalize(glm::vec3(0.0, -1.0, phi)));
		vertices.push_back(glm::normalize(glm::vec3(0.0, 1.0, phi)));
		vertices.push_back(glm::normalize(glm::vec3(0.0, -1.0, -phi)));
		vertices.push_back(glm::normalize(glm::vec3(0.0, 1.0, -phi)));
		vertices.push_back(glm::normalize(glm::vec3(phi, 0.0, -1.0)));
		vertices.push_back(glm::normalize(glm::vec3(phi, 0.0, 1.0)));
		vertices.push_back(glm::normalize(glm::vec3(-phi, 0.0, -1.0)));
		vertices.push_back(glm::normalize(glm::vec3(-phi, 0.0, 1.0)));

		triangles.push_back(glm::uvec3(0, 11, 5));
		triangles.push_back(glm::uvec3(0, 5, 1));
		triangles.push_back(glm::uvec3(0, 1, 7));
		triangles.push_back(glm::uvec3(0, 7, 10));
		triangles.push_back(glm::uvec3(0, 10, 11));
		triangles.push_back(glm::uvec3(1, 5, 9));
		triangles.push_back(glm::uvec3(5, 11, 4));
		triangles.push_back(glm::uvec3(11, 10, 2));
		triangles.push_back(glm::uvec3(10, 7, 6));
		triangles.push_back(glm::uvec3(7, 1, 8));
		triangles.push_back(glm::uvec3(3, 9, 4));
		triangles.push_back(glm::uvec3(3, 4, 2));
		triangles.push_back(glm::uvec3(3, 2, 6));
		triangles.push_back(glm::uvec3(3, 6, 8));
		triangles.push_back(glm::uvec3(3, 8, 9));
		triangles.push_back(glm::uvec3(4, 9, 5));
		triangles.push_back(glm::uvec3(2, 4, 11));
		triangles.push_back(glm::uvec3(6, 2, 10));
		triangles.push_back(glm::uvec3(8, 6, 7));
		triangles.push_back(glm::uvec3(9, 8, 1));

		int detail = 3;

		if (detail > 5) detail = 5;

		for (int i = 0; i < detail; i++)
		{
			std::vector<glm::uvec3> oldTriangles = triangles;
			triangles.clear();
			subdivideMesh(vertices, oldTriangles, vertices, triangles);
		}

		std::vector<Vertex> v;
		for (int i = 0; i < vertices.size(); i++)
		{
			float radius = 1.0;

			float lambda = atan2(vertices[i].z, vertices[i].x) + glm::pi<float>() / 2.f;
			float tau = acos(vertices[i].y / radius);

			v.emplace_back(
				vertices[i],
				glm::vec2(lambda / (2.f * glm::pi<float>()), (glm::pi<float>() - tau) / glm::pi<float>()),
				Color(0.0f, 1.0f, 1.0f, 1.0f)
			);
		}

		Model model;

		model.load(std::span<coffee::Vertex>(v.data(), v.size()), std::span<glm::uvec3>(triangles.data(), triangles.size()));

		return model;
	}
}