#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <glm/glm.hpp>
#include "subengines/render_engine.hpp"
#include "subengines/input_engine.hpp"
#include "camera.hpp"
#include <memory>

namespace se {
	class player : public renderable, public input_listener {
		using v3 = glm::vec3;
		using m4 = glm::mat4;
		private:
			float pitch = 0.0f;
			float yaw = -90.0f;
			std::unique_ptr<player_follow_camera> m_camera;
			v3 m_dir, m_up, m_side;
			void rebase() {
				m_side = glm::normalize(glm::cross(m_dir, glm::vec3(0, 1, 0)));
				m_up = glm::normalize(glm::cross(m_side, m_dir));
			}
			void adjust_camera() {
				if (m_camera != nullptr) {
					m_camera->m_pos = m_position - 8.f * m_dir + glm::vec3(0, 1, 0) * 3.f; // TODO - make this configurable
					m_camera->m_dir = m_dir;
					m_camera->m_side = m_side;
					m_camera->m_up = m_up;
				}
			}
			m4 get_rotation_matrix() const {
				return {
					m_side.x, m_up.x, m_dir.x, 0,
					m_side.y, m_up.y, m_dir.y, 0,
					m_side.z, m_up.z, m_dir.z, 0,
					0, 0, 0, 1
				};
			}
			bool should_destruct = false;
		public:
			player() = delete;
			// Giant constructor
			player(
				const v3& t_position,
				const v3& t_scale,
				const mesh& t_mesh,
				const std::list<texture>& t_textures,
				GLuint t_program
			);

			void update(input_event e) override;
			void update_mouse_offset(double x, double y) override;
			virtual glm::mat4 get_model_matrix() const override { 
				return glm::translate(glm::mat4(1.0), m_position) * get_rotation_matrix() * glm::scale(glm::mat4(1), m_scale); 
			}
			v3 get_direction() const { return m_dir; }
			v3 get_side() const { return m_side; }
			bool get_should_destruct() const { return should_destruct; }
			~player();
	
	};
}

#endif