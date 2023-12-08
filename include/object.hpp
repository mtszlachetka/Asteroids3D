#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "mesh_manager.hpp"
#include "texture_manager.hpp"
#include "camera.hpp"

namespace SE {

class object {
	friend class renderer;
	protected:
		camera* m_camera;
		GLuint m_program;
		mesh_info m_mesh;
		std::vector<texture_info> m_textures;
		glm::mat4 (*m_position_callback)(float) = [](float t) -> glm::mat4 { return glm::mat4(1); };
	public:
		object() = delete;
		object(const mesh_info& t_mesh, GLuint t_program, const std::vector<texture_info>& t_textures) :
			m_mesh(t_mesh), m_program(t_program), m_textures(t_textures) {}
		void attach_camera(camera& cam) { m_camera = &cam; }
		void set_position_callback(glm::mat4 (*callback)(float)) { m_position_callback = callback; }
		void set_mesh(const mesh_info& t_mesh) { m_mesh = t_mesh; }
		void set_program(GLuint t_program) { m_program = t_program; }
		void set_textures(const std::vector<texture_info>& t_textures) { m_textures = t_textures; }
		virtual glm::mat4 get_position(float time) const { return m_position_callback(time); }
};

}

#endif