#include "physics_module.hpp"
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <algorithm>

namespace se {

	void collision_detector::tick() {
		for (auto i = 0; i < m_objects.size(); i++) {
			for (auto j = i + 1; j < m_objects.size(); j++) {
				object* o1 = m_objects[i];
				object* o2 = m_objects[j];
				float dist2 = glm::length2(o1->m_pos - o2->m_pos);
				float treshold = (o1->m_scale_factor + o2->m_scale_factor) * (o1->m_scale_factor + o2->m_scale_factor);
				if (dist2 <= treshold) { // collision response
					float m1 = o1->m_mass, m2 = o1->m_mass;
					if (m1 == m2) {
						std::swap(o1->m_velocity, o2->m_velocity);
					} else {
						glm::vec3 u1 = (m1 - m2) / (m1 + m2) * o1->m_velocity + 2 * m2 / (m1 + m2) * o2->m_velocity;
						glm::vec3 u2 = (m2 - m1) / (m2 + m1) * o2->m_velocity + 2 * m1 / (m2 + m1) * o1->m_velocity;
						o1->m_velocity = u1;
						o2->m_velocity = u2;
					}
				}
				// std::cout << "o1 pos:\t" << o1->m_pos.x << "\t" << o1->m_pos.y << "\t" << o1->m_pos.z << "\n" <<
				// "o2 pos:\t" << o2->m_pos.x << "\t" << o2->m_pos.y << "\t" << o2->m_pos.z << "\n" <<
				// "squared distance: " << dist2 << "\n"; 
			}
		}
	}

}