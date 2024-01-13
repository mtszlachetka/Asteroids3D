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
				const se::mesh& t_mesh, 
				const std::list<se::texture>& t_textures, 
				GLuint t_program, 
				float t_health
			);
			~station();
	};
}

#endif