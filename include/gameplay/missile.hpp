#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "subengines/render_engine.hpp"
#include "subengines/physics_engine.hpp"

namespace se {
	class missile : public renderable, public rigid_body {
		using v3 = glm::vec3;
		using qu = glm::quat;
		public:
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
			) : 
			transformable(t_position, t_scale, t_orientation),
			renderable(t_position, t_scale, t_orientation, t_mesh, t_textures, t_program), 
			rigid_body(t_position, t_scale, t_orientation, t_velocity, t_mass) {}
	};
}

#endif