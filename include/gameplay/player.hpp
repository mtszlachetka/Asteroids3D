#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <glm/glm.hpp>
#include "subengines/render_engine.hpp"
#include "subengines/input_engine.hpp"
#include "subengines/collision_engine.hpp"
#include "camera.hpp"
#include <memory>
#include <iostream>

namespace se {
	class player : public renderable, public input_listener, public collidable {
		using v3 = glm::vec3;
		using m4 = glm::mat4;
		using qu = glm::quat;
		private:
			std::unique_ptr<player_follow_camera> m_camera;
			bool should_destruct = false;
			static constexpr float m_base_speed = 0.25f;
			static constexpr float m_boost_speed = 0.6f;
			float m_movespeed = m_base_speed;
			float m_anglespeed = 0.05f;
			void update_mouse_offset(double x, double y);
			static constexpr float m_base_zoom = -8.f;
			float m_camera_zoom_factor = m_base_zoom;
			void turn_on_boost();
			void turn_off_boost();
			int m_health = 100;
			int m_boost = 100;
			float m_boost_time_used = 0.f;
			bool m_boost_active = false;
			bool m_is_moving = false;
			bool m_controls_active = true;
			void turn_around();
			void evade();
		public:
			player() = delete;
			// Giant constructor
			player(
				const v3& t_position,
				const qu& t_orientation,
				const mesh& t_mesh,
				const std::list<texture>& t_textures,
				GLuint t_program,
				const bounding_sphere&,
				const obb&
			);

			obb get_obb() const override {
				return {
					m_base_obb.m_center + m_position,
					glm::toMat3(m_orientation) * m_base_obb.m_rotation,
					m_base_obb.m_extents
				};
			}

			bounding_sphere get_bounding_sphere() const override {
				return translate(m_base_sphere, m_position);
			}

			void collide_with(collidable* cl, collision_info* info);
			bool is_boosting() const { return m_boost_active; }
			bool is_moving() const { return m_is_moving; }
			void set_moving(bool t_moving) { m_is_moving = t_moving; }
			void set_controls(bool t_active) {
				m_controls_active = t_active;
			}

			void adjust_camera() {
				v3 direction = glm::toMat3(m_orientation)[2];
				m_camera->m_pos = m_position + m_camera_zoom_factor * direction + v3(0,1,0) * 3.f;
				m_camera->m_dir = direction;
				m_camera->rebase();
			}

			m4 get_camera_matrix() const {
				return m_camera->get_camera_matrix();
			}

			m4 get_perspective_matrix() const {
				return m_camera->get_perspective_matrix();
			}

			void update(input_event e) override;
			bool get_should_destruct() const { 
				if (m_time_of_destruction != 0.0) {
					if (game_clock::get_instance().get_current_frame_time() - m_time_of_destruction > m_explosion_time) {
						return true;
					}
				}
				return false;
			}
			int get_health() { return m_health; }
			int get_boost() { return m_boost; }
			void use_boost() { m_boost -= 1; }
			void recharge_boost() { m_boost += 1; }
			float get_boost_time_used() { return m_boost_time_used; }
			~player();
	
	};
}

#endif