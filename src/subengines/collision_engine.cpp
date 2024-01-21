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

				if (sq_distance <= treshold && aptr->get_time_of_destruction() == 0.f) { // collision
					mptr->notify_collision();
					aptr->notify_missile_collision();
					gameplay_engine::get_instance().add_points(100);
				}
			}
		}
		// asteroid - asteroid collision
		for (std::list<asteroid*>::iterator as1 = m_asteroids.begin(); as1 != m_asteroids.end(); as1++) {
			for (std::list<asteroid*>::iterator as2 = as1; as2 != m_asteroids.end(); as2++) {
				// assume all dimensions are equal scale
				float radius1 = (*as1)->get_scale()[0];
				float radius2 = (*as2)->get_scale()[0];

				float sq_distance = glm::length2((*as1)->get_position() - (*as2)->get_position());
				float treshold = (radius1 + radius2) * (radius1 + radius2);

				if (sq_distance <= treshold) {
					const float m1 = (*as1)->get_mass();
					const float m2 = (*as2)->get_mass();
					const glm::vec3 v1 = (*as1)->get_velocity();
					const glm::vec3 v2 = (*as2)->get_velocity();
					(*as1)->notify_asteroid_collision(v2, m2);
					(*as2)->notify_asteroid_collision(v1, m1);
				}
			}
		}

		// asteroid - station collision
		for (asteroid* aptr : m_asteroids) {
			float asteroid_bound_radius = aptr->get_scale()[0];
			float station_bound_radius = m_station->get_scale()[0] + 2.0f;

			float sq_distance = glm::length2(aptr->get_position() - m_station->get_position());
			float treshold = (asteroid_bound_radius + station_bound_radius) * (asteroid_bound_radius + station_bound_radius);

			if (sq_distance <= treshold && aptr->get_time_of_destruction() == 0.f) { // collision
				aptr->notify_station_collision();
				m_station->notify_asteroid_collision();
			}
		}
	}
}