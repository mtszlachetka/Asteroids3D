#ifndef STATION_HPP
#define STATION_HPP

#include "subengines/render_engine.hpp"
#include "subengines/collision_engine.hpp"
#include <iostream>

namespace se {
	class station : public renderable, public collidable {
		using v3 = glm::vec3;
		using qu = glm::quat;
		private:
			int m_health;
			bool should_destruct = false;
			dop14 m_cached_dop;
			bounding_sphere m_sphere;
		public:
			station() = delete;
			station(
				const v3& t_position, 
				const se::mesh& t_mesh, 
				const std::list<se::texture>& t_textures, 
				GLuint t_program, 
				float t_health
			);
			bool get_should_destruct() const { return false /* m_health <= 0 */; }
			dop14 get_dop14() {
				return m_cached_dop;
			}

			void set_bounding_sphere(const bounding_sphere& sp) { m_sphere = sp; }

			bounding_sphere get_bounding_sphere() const {
				return {m_sphere.center + m_position, m_sphere.radius};
			}

			void collide_with(collidable* cl, collision_info* info) {
				m_health--;
			}
			~station();
	};
}

#endif