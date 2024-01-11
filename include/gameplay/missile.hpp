#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "subengines/gameplay_object.hpp"

#include "subengines/renderable.hpp"
#include "subengines/gameplay_object.hpp"
#include "subengines/rigid_body.hpp"

namespace se {
	class missile : public renderable, public gameplay_object, public rigid_body {
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
				const std::list<GLuint>& t_shaders,
				const v3& t_velocity,
				float t_mass
			) : gameplay_object(t_position, t_scale, t_orientation), renderable(t_mesh, t_textures, t_shaders, 0), rigid_body(t_velocity, t_mass) {
				// render_engine::get_instance().attach(this);
				// collision_engine::get_instance().attach(this);
				// physics_engine::get_instance().attach(this);
			}
	};
}

#endif