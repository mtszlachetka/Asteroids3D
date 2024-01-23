#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "subengines/render_engine.hpp"
#include "subengines/physics_engine.hpp"
#include "subengines/collision_engine.hpp"
#include "clock.hpp"
#include <iostream>

namespace se {
	class missile : public renderable, public rigid_body, public collidable {
		using v3 = glm::vec3;
		using qu = glm::quat;
		using m4 = glm::mat4;
		private:
			bool should_destruct = false;
			float m_lifetime = 4.f;
			float m_spawn_timestamp; // exact time of creation
			dop14 m_cached_dop;
			bounding_sphere m_sphere;
		public:
			missile() = delete;
			// Giant constructor
			missile(
				const v3& t_position,
				const qu& t_orientation,
				const se::mesh& t_mesh,
				const std::list<se::texture>& t_textures,
				GLuint t_program,
				const v3& t_velocity,
				float t_mass,
				const v3& t_inertia, 
				const v3& t_angular
			);
			virtual ~missile();

			dop14 get_dop14() { return compute_dop14(m_mesh.m_vertices, get_model_matrix()); }
			void set_bounding_sphere(const bounding_sphere& sp) { m_sphere = sp; }

			bounding_sphere get_bounding_sphere() const {
				return translate(m_sphere, m_position);
			}
			void collide_with(collidable* cl, collision_info* info);

			bool get_should_destruct() const { 
				return should_destruct || game_clock::get_instance().get_current_frame_time() - m_spawn_timestamp >= m_lifetime;
			}
	};
}

#endif