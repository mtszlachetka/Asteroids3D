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
			int m_health = 100;
			bool should_destruct = false;
		public:
			station() = delete;
			station(
				const v3& t_position, 
				const se::mesh& t_mesh, 
				const std::list<se::texture>& t_textures, 
				GLuint t_program,
				const bounding_sphere&,
				const obb&, 
				int t_health
			);
			bool get_should_destruct() const { 
				return should_destruct;
			}
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

			void collide_with(collidable* cl, collision_info* info) {
				m_health -= 20;
			}
			int get_health() { return m_health; }
			~station();
	};
}

#endif