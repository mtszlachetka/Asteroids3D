#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>

namespace se
{
	struct vertex
	{
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_tex_coords;
		glm::vec3 m_tangent;
		glm::vec3 m_bitangent;
	};

	struct mesh
	{
		std::vector<vertex> m_vertices;
		std::vector<unsigned> m_indices;
		mesh() = default;
		mesh(const std::vector<vertex> &t_vertices, const std::vector<unsigned> &t_indices) : m_vertices(t_vertices), m_indices(t_indices) {}
	};

	mesh load_model(const std::string_view &filepath);

}

#endif
