#include "gameplay/station.hpp"
#include "subengines/collision_engine.hpp"

namespace se {
	station::station(
		const v3& t_position,
		const se::mesh& t_mesh, 
		const std::list<se::texture>& t_textures, 
		GLuint t_program, 
		const bounding_sphere& t_sphere,
		const obb& t_box,
		int t_health
	) : transformable(t_position, {1,0,0,0}),
		renderable(t_position, {1,0,0,0}, t_mesh, t_textures, t_program),
		collidable(t_sphere, t_box),
		m_health(t_health) {
		
	}

	station::~station() {
	}
}