#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "mesh.hpp"
#include "texture.hpp"
#include <GL/glew.h>
#include <array>

namespace se {
	class skybox {
		private:
			texture m_cubemap;
			mesh m_mesh;
			GLuint m_program;
			GLuint m_vao, m_vbo, m_ebo;
		public:
			skybox() = delete;
			skybox(const texture& t_cubemap, const mesh& t_mesh, GLuint t_program);
			texture get_cubemap() const { return m_cubemap; }
			mesh get_mesh() const { return m_mesh; }
			GLuint get_program() const { return m_program; }
			void set_cubemap(const texture& t_cubemap) { m_cubemap = t_cubemap; }
			void set_mesh(const mesh& t_mesh) { m_mesh = t_mesh; }
			void set_program(GLuint t_program) { m_program = t_program; }
			void render() const {
				glBindVertexArray(m_vao);
				glDrawElements(GL_TRIANGLES, m_mesh.m_indices.size(), GL_UNSIGNED_INT, nullptr);
				glBindVertexArray(0);
			}
			~skybox() {
				glDeleteBuffers(1, &m_vbo);
				glDeleteBuffers(1, &m_ebo);
				glDeleteVertexArrays(1, &m_vao);
			}
	};
}

#endif