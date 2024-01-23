#include "subengines/physics_engine.hpp"
#include "clock.hpp"

#include <glm/glm.hpp>

namespace se {

	rigid_body::rigid_body() : transformable() {
		physics_engine::get_instance().attach(this);
	}

	rigid_body::rigid_body(const v3& t_position, const qu& t_orientation, const v3& t_velocity, float t_mass) 
		: transformable(t_position, t_orientation), m_velocity(t_velocity), m_mass(t_mass) {
		physics_engine::get_instance().attach(this);
	}

	rigid_body::~rigid_body() {
		physics_engine::get_instance().detach(this);
	}

	void physics_engine::tick() {
		float delta_time = game_clock::get_instance().get_delta_time();
		for (rigid_body* body : m_bodies) {
			body->set_position(body->get_position() + body->get_velocity() * delta_time);
		}
	}
}
