#include "subengines/collision_engine.hpp"
#include "subengines/gameplay_engine.hpp"
#include <glm/gtx/norm.hpp>
#include <iostream>

namespace se {

	void collision_engine::tick() {
		// missile - asteroid collisions
		for (missile* mptr : m_missiles) {
			for (asteroid* aptr : m_asteroids) {
				// assume all dimensions are equal scale
				float missile_bound_radius = mptr->get_scale()[0];
				float asteroid_bound_radius = aptr->get_scale()[0];

				float sq_distance = glm::length2(mptr->get_position() - aptr->get_position());
				float treshold = (missile_bound_radius + asteroid_bound_radius) * (missile_bound_radius + asteroid_bound_radius);

				if (sq_distance <= treshold) { // collision
					mptr->notify_collision();
					aptr->notify_missile_collision();
					gameplay_engine::get_instance().add_points(100);
				}
			}
		}
	}
}