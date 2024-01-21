#include "gameplay/asteroid.hpp"
#include "subengines/collision_engine.hpp"

namespace se {

	asteroid::asteroid(
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
	rigid_body(t_position, t_scale, t_orientation, t_velocity, t_mass) {
		collision_engine::get_instance().attach(this);
	}

	asteroid::~asteroid() {
		collision_engine::get_instance().detach(this);
	}

	void asteroid::notify_asteroid_collision(const v3& other_velocity, float other_mass) {
		if (m_mass == other_mass) {
			m_velocity = other_velocity;
		} else {
			m_velocity = (m_mass - other_mass) / (m_mass + other_mass) * m_velocity + 2 * other_mass / (m_mass + other_mass) * other_velocity;
			// m_velocity = (1 / (m_mass + other_mass)) * ((m_mass - other_mass) * m_velocity + 2 * other_mass * other_velocity);
		}
	}
}