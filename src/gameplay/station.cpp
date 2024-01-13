#include "gameplay/station.hpp"
#include "subengines/collision_engine.hpp"

namespace se {
	station::station(
		const v3& t_position, 
		const v3& t_scale, 
		const se::mesh& t_mesh, 
		const std::list<se::texture>& t_textures, 
		GLuint t_program, 
		float t_health
	) : transformable(t_position, t_scale),
		renderable(t_position, t_scale, t_mesh, t_textures, t_program),
		m_health(t_health) {
		
		collision_engine::get_instance().attach(this);
	}

	station::~station() {
		collision_engine::get_instance().detach_station();
	}
}