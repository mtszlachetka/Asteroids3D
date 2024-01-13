#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "subengines/render_engine.hpp"
#include "subengines/physics_engine.hpp"
#include "clock.hpp"

namespace se {
	class missile : public renderable, public rigid_body {
		using v3 = glm::vec3;
		using qu = glm::quat;
		using m4 = glm::mat4;
		private:
			static constexpr m4 x_rotation = {
				1,	0,	0,	0,
				0,	0,	1,	0,
				0,	-1,	0,	0,
				0,	0,	0,	1
			};
			static constexpr m4 z_rotation = {
				0,	1,	0,	0,
				-1,	0,	0,	0,
				0,	0,	1,	0,
				0,	0,	0,	1
			};
			// glm::mat4 x_rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			// glm::mat4 z_rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			v3 m_dir, m_side;
			m4 get_rotation_matrix() const {
				return m4({
					m_side.x,	0,	m_dir.x,	0,
					m_side.y,	1,	m_dir.y,	0,
					m_side.z,	0,	m_dir.z,	0,
					0,			0,	0,			1

				}) * z_rotation * x_rotation;
			}
			bool should_destruct = false;
			float m_lifetime = 4.f;
			float m_spawn_timestamp; // exact time of creation
		public:
			missile() = delete;
			// Giant constructor
			missile(
				const v3& t_position,
				const v3& t_scale,
				const v3& t_direction,
				const v3& t_side,
				const se::mesh& t_mesh,
				const std::list<se::texture>& t_textures,
				GLuint t_program,
				const v3& t_velocity,
				float t_mass
			);
			virtual m4 get_model_matrix() const override { return glm::translate(m4(1.f), m_position) * get_rotation_matrix() * glm::scale(m4(1.f), m_scale); }
			virtual ~missile();
			void notify_collision() { should_destruct = true; } // same response for all collisions
			bool get_should_destruct() const { 
				return should_destruct || game_clock::get_instance().get_current_frame_time() - m_spawn_timestamp >= m_lifetime;
			}
	};
}

#endif