#include "gameplay/asteroid.hpp"
#include "gameplay/missile.hpp"
#include "gameplay/player.hpp"
#include "gameplay/station.hpp"
#include "subengines/collision_engine.hpp"
#include "subengines/gameplay_engine.hpp"

namespace se {

	asteroid::asteroid(
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
	}

	asteroid::~asteroid() {
	}

	void asteroid::collide_with(collidable* cl, collision_info* t_info) {
		// RTTI
		// if asteroid
		asteroid* asptr = dynamic_cast<asteroid*>(cl);
		if (asptr != nullptr) {
			asteroid_collision_info* info = dynamic_cast<asteroid_collision_info*>(t_info);
			v3 other_velocity = info->t_velocity;
			float other_mass = info->t_mass;
			if (m_mass == other_mass) {
			m_velocity = other_velocity;
			} else {
				m_velocity = (m_mass - other_mass) / (m_mass + other_mass) * m_velocity + 2 * other_mass / (m_mass + other_mass) * other_velocity;
				// m_velocity = (1 / (m_mass + other_mass)) * ((m_mass - other_mass) * m_velocity + 2 * other_mass * other_velocity);
			}
			return;
		}

		// if missile
		missile* mptr = dynamic_cast<missile*>(cl);
		if (mptr != nullptr) {
			m_time_of_destruction = game_clock::get_instance().get_current_frame_time();
			m_occluder = false;
			collision_engine::get_instance().detach(this);
			gameplay_engine::get_instance().add_points(100);
			return;
		}

		// if station
		station* sptr = dynamic_cast<station*>(cl);
		if (sptr != nullptr) {
			m_time_of_destruction = game_clock::get_instance().get_current_frame_time();
			m_occluder = false;
			collision_engine::get_instance().detach(this);
			return;
		}		
	}

}