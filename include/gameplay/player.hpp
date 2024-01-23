#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <glm/glm.hpp>
#include "subengines/render_engine.hpp"
#include "subengines/input_engine.hpp"
#include "subengines/collision_engine.hpp"
#include "camera.hpp"
#include <memory>

namespace se {
	class player : public renderable, public input_listener, public collidable {
		using v3 = glm::vec3;
		using m4 = glm::mat4;
		using qu = glm::quat;
		private:
			std::unique_ptr<player_follow_camera> m_camera;
			bool should_destruct = false;
			static constexpr float m_base_speed = 0.25f;
			float m_movespeed = m_base_speed;
			float m_anglespeed = 0.05f;
			void update_mouse_offset(double x, double y);
			static constexpr float m_base_zoom = -8.f;
			float m_camera_zoom_factor = m_base_zoom;
			dop14 m_cached_dop;
			bounding_sphere m_sphere;

		public:
			player() = delete;
			// Giant constructor
			player(
				const v3& t_position,
				const qu& t_orientation,
				const mesh& t_mesh,
				const std::list<texture>& t_textures,
				GLuint t_program
			);

			dop14 get_dop14() {
				return compute_dop14(m_mesh.m_vertices, get_model_matrix());
			}

			void set_bounding_sphere(const bounding_sphere& sp) { m_sphere = sp; }

			bounding_sphere get_bounding_sphere() const {
				return translate(m_sphere, m_position);
			}

			void collide_with(collidable* cl, collision_info* info) {

			}

			void adjust_camera() {
				v3 direction = glm::toMat4(m_orientation) * glm::vec4(0,0,1,0);
				m_camera->m_pos = m_position + m_camera_zoom_factor * direction + v3(0,1,0) * 3.f;
				m_camera->m_dir = direction;
				m_camera->rebase();
			}

			void update(input_event e) override;
			bool get_should_destruct() const { return should_destruct; }
			~player();
	
	};
}

#endif