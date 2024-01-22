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
			bool should_destruct = false;
			float m_lifetime = 4.f;
			float m_spawn_timestamp; // exact time of creation
		public:
			missile() = delete;
			// Giant constructor
			missile(
				const v3& t_position,
				const v3& t_scale,
				const qu& t_orientation,
				const se::mesh& t_mesh,
				const std::list<se::texture>& t_textures,
				GLuint t_program,
				const v3& t_velocity,
				float t_mass
			);
			virtual ~missile();
			void notify_collision() { should_destruct = true; } // same response for all collisions
			bool get_should_destruct() const { 
				return should_destruct || game_clock::get_instance().get_current_frame_time() - m_spawn_timestamp >= m_lifetime;
			}
	};
}

#endif