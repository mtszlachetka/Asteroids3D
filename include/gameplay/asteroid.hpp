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
		public:
			asteroid() = delete;
			// Giant constructor
			asteroid(
				const v3& t_position,
				const qu& t_orientation,
				const se::mesh& t_mesh,
				const std::list<se::texture>& t_textures,
				GLuint t_program,
				const v3& t_velocity,
				float t_mass,
				const v3& t_inertia, 
				const v3& t_angular,
				const bounding_sphere& t_sphere,
				const obb& t_box
			);
			virtual ~asteroid();

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