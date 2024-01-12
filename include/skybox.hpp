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
		public:
			skybox() = delete;
			skybox(const texture& t_cubemap, const mesh& t_mesh, GLuint t_program) : m_cubemap(t_cubemap), m_mesh(t_mesh), m_program(t_program) {}
			texture get_cubemap() const { return m_cubemap; }
			mesh get_mesh() const { return m_mesh; }
			GLuint get_program() const { return m_program; }
			void set_cubemap(const texture& t_cubemap) { m_cubemap = t_cubemap; }
			void set_mesh(const mesh& t_mesh) { m_mesh = t_mesh; }
			void set_program(GLuint t_program) { m_program = t_program; }
	};
}

#endif