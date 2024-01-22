#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "subengines/render_engine.hpp"
#include "subengines/physics_engine.hpp"
#include "subengines/collision_engine.hpp"
#include <iostream>
#include <tuple>
#include <memory>

namespace se {
	class asteroid : public renderable, public rigid_body, public collidable {
		using v3 = glm::vec3;
		using qu = glm::quat;
		private:
			static constexpr float m_explosion_time = 0.6f;
			dop14 m_cached_dop;
			bounding_sphere m_sphere;
		public:
			asteroid() = delete;
			// Giant constructor
			asteroid(
				const v3& t_position,
				const v3& t_scale,
				const qu& t_orientation,
				const se::mesh& t_mesh,
				const std::list<se::texture>& t_textures,
				GLuint t_program,
				const v3& t_velocity,
				float t_mass
			);
			virtual ~asteroid();

			dop14 get_dop14() {
				return m_cached_dop;
			}

			bounding_sphere get_bounding_sphere() {
				return {m_position, m_scale[0]};
			}

			void collide_with(collidable* cl, collision_info* info);

			std::shared_ptr<collision_info> get_collision_info() {
				return std::make_shared<asteroid_collision_info>(m_velocity, m_mass);
			}

			bool get_should_destruct() const { 
				if (m_time_of_destruction != 0.0) {
					if (game_clock::get_instance().get_current_frame_time() - m_time_of_destruction > m_explosion_time) {
						return true;
					}
				}
				return false;
			}
	};
}

#endif