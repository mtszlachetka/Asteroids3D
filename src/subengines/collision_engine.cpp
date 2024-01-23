#include "subengines/collision_engine.hpp"
#include "subengines/gameplay_engine.hpp"
#include <glm/gtx/norm.hpp>
#include <tuple>
#include <vector>
#include <limits>

namespace se {

	collidable::collidable() {
		collision_engine::get_instance().attach(this);
	}
	collidable::~collidable() {
		collision_engine::get_instance().detach(this);
	}

	static inline bool intersection(const bounding_sphere& sp1, const bounding_sphere& sp2) {
		auto&& [c1, r1] = sp1;
		auto&& [c2, r2] = sp2;

		float sq_distance = glm::length2(c1 - c2);
		float treshold = (r1 + r2) * (r1 + r2);

		return sq_distance <= treshold;
	}

	static inline bool intersection(const dop14& kd1, const dop14& kd2) {
		for (int i = 0; i < 7; i++) {
			if (kd1.min[i] > kd2.max[i] || kd1.max[i] < kd2.min[i])
				return false;
		}
		return true;
	}

	void collision_engine::tick() {
		static std::vector<std::tuple<collidable*, collidable*, std::shared_ptr<collision_info>, std::shared_ptr<collision_info>>> to_notify;
		for (std::list<collidable*>::iterator cptr1 = m_collidables.begin(); cptr1 != m_collidables.end(); cptr1++) {
			for (std::list<collidable*>::iterator cptr2 = std::next(cptr1); cptr2 != m_collidables.end(); cptr2++) {
				// bounding sphere test
				collidable* col1 = *cptr1;
				collidable* col2 = *cptr2;

				if (intersection(col1->get_bounding_sphere(), col2->get_bounding_sphere())) {
					if (intersection(col1->get_dop14(), col2->get_dop14())) {
						asteroid* a1 = dynamic_cast<asteroid*>(col1);
						to_notify.push_back({col1, col2, col1->get_collision_info(), col2->get_collision_info()});
					}					
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
	dop14 compute_dop14(const std::vector<se::vertex>& t_vertices, const glm::mat4& t_transform) {
		dop14 dop;

		for (int i = 0; i < 7; i++) {
			dop.min[i] = std::numeric_limits<float>::max();
			dop.max[i] = -std::numeric_limits<float>::max();
		}

		float value;
		for (const vertex& vert : t_vertices) {
			const glm::vec3& pos = t_transform * glm::vec4(vert.m_position, 1.f);
			// {1, 0, 0}
			value = pos.x;
			if (value < dop.min[0]) dop.min[0] = value;
			if (value > dop.max[0]) dop.max[0] = value;
			// {0, 1, 0}
			value = pos.y;
			if (value < dop.min[1]) dop.min[1] = value;
			if (value > dop.max[1]) dop.max[1] = value;
			// {0, 0, 1}
			value = pos.z;
			if (value < dop.min[2]) dop.min[2] = value;
			if (value > dop.max[2]) dop.max[2] = value;
			// {1, 1, 1}
			value = pos.x + pos.y + pos.z;
			if (value < dop.min[3]) dop.min[3] = value;
			if (value > dop.max[3]) dop.max[3] = value;
			// {1, 1, -1}
			value = pos.x + pos.y - pos.z;
			if (value < dop.min[4]) dop.min[4] = value;
			if (value > dop.max[4]) dop.max[4] = value;
			// {1, -1, 1}
			value = pos.x - pos.y + pos.z;
			if (value < dop.min[5]) dop.min[5] = value;
			if (value > dop.max[5]) dop.max[5] = value;
			// {1, -1, -1}
			value = pos.x - pos.y - pos.z;
			if (value < dop.min[6]) dop.min[6] = value;
			if (value > dop.max[6]) dop.max[6] = value;
		}
		return dop;
	}
}


