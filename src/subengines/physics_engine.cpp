#include "subengines/physics_engine.hpp"
#include "clock.hpp"

#include <glm/glm.hpp>

namespace se
{

	rigid_body::rigid_body(const v3 &t_position, const qu &t_orientation, const v3 &t_velocity, float t_mass, const v3 &t_inertia, const v3 &t_angular)
		: transformable(t_position, t_orientation), m_velocity(t_velocity), m_mass(t_mass), m_inertia(t_inertia), m_angular_velocity(t_angular)
	{
		physics_engine::get_instance().attach(this);
	}

	rigid_body::~rigid_body()
	{
		physics_engine::get_instance().detach(this);
	}

	void physics_engine::tick()
	{
		float delta_time = game_clock::get_instance().get_delta_time();
		for (rigid_body *body : m_bodies)
		{
			body->set_position(body->get_position() + body->get_velocity() * delta_time);
			glm::vec3 I = body->get_inertia();
			glm::mat3 inv_inertia = glm::mat3({1.f / I.x, 0.f, 0.f,
											   0.f, 1.f / I.y, 0.f,
											   0.f, 0.f, 1.f / I.z});
			glm::vec3 ang_vel = inv_inertia * body->get_angular();
			glm::quat omega = glm::normalize(glm::quat(0, ang_vel.x, ang_vel.y, ang_vel.z));

			glm::quat ori = body->get_orientation();
			body->set_orientation(glm::normalize(ori + 0.5f * omega * ori * delta_time));
		}
	}
}
