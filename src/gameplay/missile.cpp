#include "gameplay/missile.hpp"
#include "subengines/collision_engine.hpp"
#include "clock.hpp"

namespace se {
	missile::missile(
		const v3& t_position,
		const v3& t_scale,
		const v3& t_direction,
		const v3& t_side,
		const se::mesh& t_mesh,
		const std::list<se::texture>& t_textures,
		GLuint t_program,
		const v3& t_velocity,
		float t_mass
	) : 
	transformable(t_position, t_scale),
	renderable(t_position, t_scale, t_mesh, t_textures, t_program), 
	rigid_body(t_position, t_scale, t_velocity, t_mass),
	m_dir(t_direction), m_side(t_side) {
		m_spawn_timestamp = game_clock::get_instance().get_current_frame_time();
		collision_engine::get_instance().attach(this);
	}

	missile::~missile() {
		collision_engine::get_instance().detach(this);
	}
}