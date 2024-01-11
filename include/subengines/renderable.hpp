#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "mesh.hpp"
#include "texture.hpp"
#include <GL/glew.h>
#include <list>

namespace se {
	class renderable {
		protected:
			se::mesh m_mesh;
			std::list<se::texture> m_textures;
			std::list<GLuint> m_shaders;
			GLuint m_active_shader {0};
		public:
			renderable() {}
			renderable(const se::mesh& t_mesh, const std::list<se::texture>& t_textures, const std::list<GLuint>& t_shaders, GLuint t_active = 0)
				: m_mesh(t_mesh), m_textures(t_textures), m_shaders(t_shaders), m_active_shader(t_active) {}
			se::mesh get_mesh() const { return m_mesh; }
			std::list<se::texture> get_textures() const { return m_textures; }
			std::list<GLuint> get_shaders() const { return m_shaders; }
			GLuint get_active_shader() const { return m_active_shader; }
			void set_mesh(const se::mesh& t_mesh) { m_mesh = t_mesh; }
			void set_textures(const std::list<se::texture>& t_textures) { m_textures = t_textures; }
			void set_shaders(const std::list<GLuint>& t_shaders) { m_shaders = t_shaders; }
	};
}

#endif