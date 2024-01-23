#include "gameplay/missile.hpp"
#include "subengines/collision_engine.hpp"
#include "clock.hpp"
#include "gameplay/player.hpp"

namespace se {
	missile::missile(
		const v3& t_position,
		const qu& t_orientation,
		const se::mesh& t_mesh,
		const std::list<se::texture>& t_textures,
		GLuint t_program,
		const v3& t_velocity,
		float t_mass
	) : 
	transformable(t_position, t_orientation),
	renderable(t_position, t_orientation, t_mesh, t_textures, t_program), 
	rigid_body(t_position, t_orientation, t_velocity, t_mass) {
		m_spawn_timestamp = game_clock::get_instance().get_current_frame_time();
	}

	missile::~missile() {
	}

	void missile::collide_with(collidable* cl, collision_info* info) {

		player* pptr = dynamic_cast<player*>(cl);
		if (pptr != nullptr) return; // don't collide with player
		should_destruct = true;
		collision_engine::get_instance().detach(this);
	}
}