#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "mesh.hpp"
#include "texture_manager.hpp"
#include "camera.hpp"

namespace se {

class object {
	friend class scene;
	friend class light_source;
	friend class directional_light;
	friend class punctual_light;
	protected:
		camera* m_camera;
		GLuint m_program;
		mesh m_mesh;
		std::vector<texture_info> m_textures;
		glm::mat4 m_scale = glm::mat4(1);
		glm::mat4 (*m_position_callback)(void) = [](void) -> glm::mat4 { return glm::mat4(1); };
		bool m_transparent;
	public:
		object() = delete;
		object(const mesh& t_mesh, GLuint t_program, const std::vector<texture_info>& t_textures, bool t_transparent = false) :
			m_mesh(t_mesh), m_program(t_program), m_textures(t_textures), m_transparent(t_transparent) {}
		void attach_camera(camera& cam) { m_camera = &cam; }
		void set_position_callback(glm::mat4 (*callback)(void)) { m_position_callback = callback; }
		void set_mesh(const mesh& t_mesh) { m_mesh = t_mesh; }
		void set_program(GLuint t_program) { m_program = t_program; }
		void set_textures(const std::vector<texture_info>& t_textures) { m_textures = t_textures; }
		void set_scale(const glm::mat4& t_scale) { m_scale = t_scale; }
		virtual glm::mat4 get_model_matrix() const { return m_position_callback() * m_scale; }
		virtual ~object() {}
		// virtual void render();
};
class controllable_object : public object {
		using v3 = glm::vec3;
		friend class io_processor;
		private:
			v3 m_pos, m_dir, m_side, m_up;
			float m_movespeed, m_anglespeed;
			glm::mat4 get_rotation_matrix() const {
				return {
					m_side.x, m_up.x, m_dir.x, 0,
					m_side.y, m_up.y, m_dir.y, 0,
					m_side.z, m_up.z, m_dir.z, 0,
					0, 0, 0, 1
				};
			}
		public:
			controllable_object(const se::mesh& t_mesh, GLuint t_program, const std::vector<se::texture_info>& tex, const v3& t_pos, const v3& t_dir, 
					float mspeed, float aspeed) : object(t_mesh, t_program, tex), m_pos(t_pos), m_dir(t_dir), 
						m_movespeed(mspeed), m_anglespeed(aspeed) { rebase(); }
			virtual glm::mat4 get_model_matrix() const { return glm::translate(glm::mat4(1.0), m_pos) * get_rotation_matrix() * m_scale; }
			void rebase() {
				m_side = glm::normalize(glm::cross(m_dir, {0, 1, 0}));
				m_up = glm::normalize(glm::cross(m_side, m_dir));
			}
			~controllable_object() {}
	};

}

#endif