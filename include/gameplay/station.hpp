#ifndef STATION_HPP
#define STATION_HPP

#include "subengines/render_engine.hpp"

namespace se {
	class station : public renderable {
		using v3 = glm::vec3;
		using qu = glm::quat;
		private:
			float m_health;
		public:
			station() = delete;
			station(
				const v3& t_position, 
				const v3& t_scale, 
				const qu& t_orientation, 
				const se::mesh& t_mesh, 
				const std::list<se::texture>& t_textures, 
				GLuint t_program, 
				float t_health
			) : transformable(t_position, t_scale, t_orientation),
				renderable(t_position, t_scale, t_orientation, t_mesh, t_textures, t_program),
				m_health(t_health) {}
			float get_health() const { return m_health; }
	};
}

#endif