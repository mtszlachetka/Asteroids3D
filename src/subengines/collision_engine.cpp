#include "subengines/collision_engine.hpp"
#include "subengines/gameplay_engine.hpp"
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <utility>
#include <tuple>

namespace se {

	collidable::collidable() {
		collision_engine::get_instance().attach(this);
	}
	collidable::~collidable() {
		collision_engine::get_instance().detach(this);
	}

	
	void collision_engine::tick() {
		static std::list<std::tuple<collidable*, collidable*, std::shared_ptr<collision_info>, std::shared_ptr<collision_info>>> to_notify;
		for (std::list<collidable*>::iterator cptr1 = m_collidables.begin(); cptr1 != m_collidables.end(); cptr1++) {
			for (std::list<collidable*>::iterator cptr2 = std::next(cptr1); cptr2 != m_collidables.end(); cptr2++) {
				// bounding sphere test
				collidable* col1 = *cptr1;
				collidable* col2 = *cptr2;
				auto&& [c1, r1] = col1->get_bounding_sphere();
				auto&& [c2, r2] = col2->get_bounding_sphere();

				float sq_distance = glm::length2(c1 - c2);
				float treshold = (r1 + r2) * (r1 + r2);

				if (sq_distance <= treshold) {
					asteroid* a1 = dynamic_cast<asteroid*>(col1);
					to_notify.push_back({col1, col2, col1->get_collision_info(), col2->get_collision_info()});
				}
			}
		}
		for (auto [col1, col2, info_for_2, info_for_1] : to_notify) {
			col1->collide_with(col2, info_for_1.get());
			col2->collide_with(col1, info_for_2.get());
		}
		to_notify.clear();
	}
}