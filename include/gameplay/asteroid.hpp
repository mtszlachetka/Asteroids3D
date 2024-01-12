#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "subengines/render_engine.hpp"
#include "subengines/physics_engine.hpp"

namespace se {
	class asteroid : public renderable, public rigid_body {
		using v3 = glm::vec3;
		using qu = glm::quat;
		public:
			asteroid() : transformable(), renderable(), rigid_body() {}
			// Giant constructor
			asteroid(
				const v3& t_position,
				const v3& t_scale,
				const se::mesh& t_mesh,
				const std::list<se::texture>& t_textures,
				GLuint t_program,
				const v3& t_velocity,
				float t_mass
			) : 
			transformable(t_position, t_scale),
			renderable(t_position, t_scale, t_mesh, t_textures, t_program), 
			rigid_body(t_position, t_scale, t_velocity, t_mass) {}
	};
}

#endif