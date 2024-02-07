#include "gameplay/station.hpp"
#include "subengines/collision_engine.hpp"
#include "gameplay/asteroid.hpp"

namespace se
{
	station::station(
		const v3 &t_position,
		const se::mesh &t_mesh,
		const std::list<se::texture> &t_textures,
		GLuint t_program,
		const bounding_sphere &t_sphere,
		const obb &t_box,
		int t_health) : transformable(t_position, {1, 0, 0, 0}),
						renderable(t_position, {1, 0, 0, 0}, t_mesh, t_textures, t_program),
						collidable(t_sphere, t_box),
						m_health(t_health)
	{
	}

	station::~station()
	{
	}

	void station::collide_with(collidable *cl, collision_info *info)
	{
		asteroid *a_ptr = dynamic_cast<asteroid *>(cl);
		if (a_ptr != nullptr)
		{
			asteroid_collision_info *a_info = dynamic_cast<asteroid_collision_info *>(info);
			float asteroid_momentum = glm::length(a_info->t_velocity) * a_info->t_mass;
			float damage = 0.1f * asteroid_momentum / game_clock::get_instance().get_delta_time();
			m_health -= damage;
			if (m_health <= 0 && m_time_of_destruction == 0.0f)
			{
				m_time_of_destruction = game_clock::get_instance().get_current_frame_time();
			}
			return;
		}
		m_health -= 20;
		if (m_health <= 0 && m_time_of_destruction == 0.0f)
		{
			m_time_of_destruction = game_clock::get_instance().get_current_frame_time();
		}
	}
}