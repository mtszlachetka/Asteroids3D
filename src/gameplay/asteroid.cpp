#include "gameplay/asteroid.hpp"
#include "subengines/collision_engine.hpp"

namespace se {

	asteroid::asteroid(
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
	rigid_body(t_position, t_scale, t_velocity, t_mass) {
		collision_engine::get_instance().attach(this);
	}

	asteroid::~asteroid() {
		collision_engine::get_instance().detach(this);
	}
}