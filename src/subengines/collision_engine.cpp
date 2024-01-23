#include "subengines/collision_engine.hpp"
#include "subengines/gameplay_engine.hpp"
#include <glm/gtx/norm.hpp>
#include <tuple>
#include <vector>

namespace se {

	collidable::collidable() {
		collision_engine::get_instance().attach(this);
	}
	collidable::~collidable() {
		collision_engine::get_instance().detach(this);
	}

	
	void collision_engine::tick() {
		static std::vector<std::tuple<collidable*, collidable*, std::shared_ptr<collision_info>, std::shared_ptr<collision_info>>> to_notify;
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

	bounding_sphere compute_bounding_sphere(const std::vector<se::vertex> &t_vertices) {
		using v3 = glm::vec3;
		// uses Ritter algorithm

		// First pass - find extremal points along x,y,z axes and calculate distance
		v3 minx = v3(0.f), maxx = v3(0.f), miny = v3(0.f), maxy = v3(0.f), minz = v3(0.f), maxz = v3(0.f);
		for (const vertex& vert : t_vertices) {
			const v3& pos = vert.m_position;
			if (pos.x < minx.x) minx = pos;
			if (pos.x > maxx.x) maxx = pos;
			if (pos.y < miny.y) miny = pos;
			if (pos.y > maxy.y) maxy = pos;
			if (pos.z < minz.z) minz = pos;
			if (pos.z > maxz.z) maxz = pos;
		}

		float sq_x_distance = glm::length2(maxx - minx);
		float sq_y_distance = glm::length2(maxy - miny);
		float sq_z_distance = glm::length2(maxz - minz);

		v3 max_point = maxx, min_point = minx;
		if (sq_y_distance > sq_x_distance && sq_y_distance > sq_z_distance) {
			max_point = maxy;
			min_point = miny;
		}
		if (sq_z_distance > sq_x_distance && sq_z_distance > sq_y_distance) {
			max_point = maxz;
			min_point = minz;
		}

		v3 midpoint = (min_point + max_point) * 0.5f;
		float radius = glm::length(max_point - midpoint);

		// Second pass - check if any points lay outside of the sphere and adjust accordingly
		for (const vertex& vert : t_vertices) {
			const v3& pos = vert.m_position;
			v3 difference = pos - midpoint;
			float sq_distance = glm::dot(difference, difference);
			if (sq_distance > radius * radius) {
				float distance = std::sqrt(sq_distance);
				float new_radius = (radius + distance) * 0.5f;
				radius = new_radius;
				midpoint += (new_radius - radius);
			}
		}
 
		return {midpoint, radius};
	}
	dop14 compute_dop_14(const std::vector<se::vertex>& t_vertices) {
		// TODO
		return dop14();
	}
}


