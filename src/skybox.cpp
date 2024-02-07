#include "skybox.hpp"

namespace se {
	skybox::skybox(const texture& t_cubemap, const mesh& t_mesh, GLuint t_program) : m_cubemap(t_cubemap), m_mesh(t_mesh), m_program(t_program) {
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		unsigned num_vertices = m_mesh.m_vertices.size();
		unsigned num_indices = m_mesh.m_indices.size();

		unsigned 
			data_size = sizeof(float) * 3,
			normal_size = sizeof(float) * 3,
			tex_size = sizeof(float) * 2,
			tangent_size = sizeof(float) * 3,
			bitangent_size = sizeof(float) * 3;

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned), m_mesh.m_indices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(vertex), m_mesh.m_vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(data_size));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(data_size + normal_size));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(data_size + normal_size + tex_size));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(data_size + normal_size + tex_size + tangent_size));

		glBindVertexArray(0);
	}
}