#include "physics_module.hpp"
#include <glm/gtx/norm.hpp>
#include <iostream>

namespace se {

	void collision_detector::tick() {
		for (auto i = 0; i < m_objects.size(); i++) {
			for (auto j = i + 1; j < m_objects.size(); j++) {
				object* o1 = m_objects[i];
				object* o2 = m_objects[j];
				float dist2 = glm::length2(o1->m_pos - o2->m_pos);
				float treshold = (o1->m_scale_factor + o2->m_scale_factor) * (o1->m_scale_factor + o2->m_scale_factor);
				if (dist2 <= treshold) {
					o1->m_velocity = glm::vec3(0);
					o2->m_velocity = glm::vec3(0);
				}
				// std::cout << "o1 pos:\t" << o1->m_pos.x << "\t" << o1->m_pos.y << "\t" << o1->m_pos.z << "\n" <<
				// "o2 pos:\t" << o2->m_pos.x << "\t" << o2->m_pos.y << "\t" << o2->m_pos.z << "\n" <<
				// "squared distance: " << dist2 << "\n"; 
			}
		}
	}

}