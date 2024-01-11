#ifndef STATION_HPP
#define STATION_HPP

#include "subengines/gameplay_object.hpp"

#include "subengines/renderable.hpp"
#include "subengines/gameplay_object.hpp"
#include "subengines/rigid_body.hpp"

namespace se {
	class station : public gameplay_object, public renderable {
		using v3 = glm::vec3;
		using qu = glm::quat;
		private:
			float health;
		public:
			// Giant constructor
			station(
				const v3& t_position,
				const v3& t_scale,
				const qu& t_orientation,
				const se::mesh& t_mesh,
				const std::list<se::texture>& t_textures,
				const std::list<GLuint>& t_shaders
			) : gameplay_object(t_position, t_scale, t_orientation), renderable(t_mesh, t_textures, t_shaders) {}
	};
}

#endif