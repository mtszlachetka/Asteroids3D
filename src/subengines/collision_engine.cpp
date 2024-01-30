#include "subengines/collision_engine.hpp"
#include "subengines/gameplay_engine.hpp"
#include <glm/gtx/norm.hpp>
#include <tuple>
#include <vector>
#include <limits>
#include "subengines/debug.hpp"
#include "math.hpp"

namespace se {

	collidable::collidable(const bounding_sphere& t_sphere, const obb& t_box) : m_base_sphere(t_sphere), m_base_obb(t_box) {
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
					if (intersection(col1->get_obb(), col2->get_obb())) {
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

	obb compute_obb(const std::vector<se::vertex>& t_vertices) {
		using v3 = glm::vec3;
		// find centroid
		float one_over_n = 1.f / t_vertices.size();
		v3 sumvec(0.f);

		for (const vertex& vert : t_vertices) {
			const v3& pos = vert.m_position;
			sumvec += pos;
		}

		v3 centroid = sumvec * one_over_n;

		// build covariance matrix
		glm::mat3 cov(0.f);
		for (const vertex& vert : t_vertices) {
			const v3& pos = vert.m_position;
			
			cov[0][0] += (pos.x - centroid.x) * (pos.x - centroid.x); // xx
			cov[0][1] += (pos.x - centroid.x) * (pos.y - centroid.y); // xy
			cov[0][2] += (pos.x - centroid.x) * (pos.z - centroid.z); // xz

			cov[1][0] += (pos.y - centroid.y) * (pos.x - centroid.x); // yx
			cov[1][1] += (pos.y - centroid.y) * (pos.y - centroid.y); // yy
			cov[2][2] += (pos.y - centroid.y) * (pos.z - centroid.z); // yz

			cov[2][0] += (pos.z - centroid.z) * (pos.x - centroid.x); // zx
			cov[2][1] += (pos.z - centroid.z) * (pos.y - centroid.y); // zy
			cov[2][2] += (pos.z - centroid.z) * (pos.z - centroid.z); // zz
		}

		cov *= one_over_n;

		// find eigenvectors of unitary matrix
		glm::mat3 A = cov * glm::transpose(cov);
		auto&& [eigenval1, eigenvec1] = math::power_eigen(A, A[0]);

		glm::mat3 Ainv = glm::inverse(A);
		auto&& [eigenval2, eigenvec2] = math::power_eigen(Ainv, A[1]);

		v3 eigenvec3 = glm::normalize(glm::cross(eigenvec1, eigenvec2));

		// U matrix in SVD
		glm::mat3 U = glm::transpose(glm::mat3(eigenvec1, eigenvec2, eigenvec3));

		// find min-max along each axis

		glm::mat3 inv_rotation = glm::transpose(U); // inverse is transpose, since orthonormal

		float maxf = std::numeric_limits<float>::max();

		float minx = maxf, maxx = -maxf, miny = maxf, maxy = -maxf, minz = maxf, maxz = -maxf;
		for (const vertex& vert : t_vertices) {
			const v3& pos = inv_rotation * vert.m_position; // align with standard basis
			if (pos.x < minx) minx = pos.x;
			if (pos.x > maxx) maxx = pos.x;
			if (pos.y < miny) miny = pos.y;
			if (pos.y > maxy) maxy = pos.y;
			if (pos.z < minz) minz = pos.z;
			if (pos.z > maxz) maxz = pos.z;
		}

		v3 min(minx, miny, minz);
		v3 max(maxx, maxy, maxz);

		glm::vec3 center = 0.5f * (min + max);

		glm::vec3 extents(max.x - center.x, max.y - center.y, max.z - center.z);

		return {center, U, extents};
	}

	bool intersection(const obb& t_box_a, const obb& t_box_b) {
		glm::mat3 R; // rotation matrix from B to A coordinate frame
		const glm::mat3& A = t_box_a.m_rotation;
		const glm::mat3& B = t_box_b.m_rotation;
		for (int i = 0; i < 3; i++) 
			for (int j = 0; j < 3; j++)
				R[i][j] = glm::dot(A[i], B[j]);
		
		glm::vec3 T = R * (t_box_b.m_center - t_box_a.m_center); // distance vector between centers in A's coordinate drame

		
		static const float epsilon = 0.000001f;
		glm::mat3 absR = glm::abs(R) + epsilon; // matrix of absulute values of R, add eps for numeric reasons
		
		// SAT time
		float ra, rb;
		// test A's axes
		for (int i = 0; i < 3; i++) {
			ra = t_box_a.m_extents[i];
			rb = glm::dot(t_box_b.m_extents, absR[i]);
			if (glm::abs(T[i]) > ra + rb) return false; // separating axis found
		}
		// same for B
		for (int i =0; i < 3; i++) {
			ra = glm::dot(t_box_a.m_extents, absR[i]);
			rb = t_box_b.m_extents[i];
			if (glm::abs(T[i]) > ra + rb) return false; // separating axis found
		}

		// A0 X B0
		ra = t_box_a.m_extents[1] * absR[2][0] + t_box_a.m_extents[2] * absR[1][0];
		rb = t_box_b.m_extents[1] * absR[0][2] + t_box_b.m_extents[2] * absR[0][1];
		if (glm::abs(T[2] * R[1][0] - T[1] * R[2][0]) > ra + rb) return false;

		// AO X B1
		ra = t_box_a.m_extents[1] * absR[2][1] + t_box_a.m_extents[2] * absR[1][1];
		rb = t_box_b.m_extents[0] * absR[0][2] + t_box_b.m_extents[2] * absR[0][0];
		if (glm::abs(T[2] * R[1][1] - T[1] * R[2][1]) > ra + rb) return false;

		// AO X B2
		ra = t_box_a.m_extents[1] * absR[2][2] + t_box_a.m_extents[2] * absR[1][2];
		rb = t_box_b.m_extents[0] * absR[0][1] + t_box_b.m_extents[1] * absR[0][0];
		if (glm::abs(T[2] * R[1][2] - T[1] * R[2][2]) > ra + rb) return false;

		// A1 X B0
		ra = t_box_a.m_extents[0] * absR[2][0] + t_box_a.m_extents[2] * absR[0][0];
		rb = t_box_b.m_extents[1] * absR[1][2] + t_box_b.m_extents[2] * absR[1][1];
		if (glm::abs(T[0] * R[2][0] - T[2] * R[0][0]) > ra + rb) return false;

		// A1 X B1
		ra = t_box_a.m_extents[0] * absR[2][1] + t_box_a.m_extents[2] * absR[0][1];
		rb = t_box_b.m_extents[0] * absR[1][2] + t_box_b.m_extents[2] * absR[1][0];
		if (glm::abs(T[0] * R[2][1] - T[2] * R[0][1]) > ra + rb) return false;

		// A1 X B2
		ra = t_box_a.m_extents[0] * absR[2][2] + t_box_a.m_extents[2] * absR[0][2];
		rb = t_box_b.m_extents[0] * absR[1][1] + t_box_b.m_extents[1] * absR[1][0];
		if (glm::abs(T[0] * R[2][2] - T[2] * R[0][2]) > ra + rb) return false;

		// A2 X B0
		ra = t_box_a.m_extents[0] * absR[1][0] + t_box_a.m_extents[1] * absR[0][0];
		rb = t_box_b.m_extents[1] * absR[2][2] + t_box_b.m_extents[2] * absR[2][1];
		if (glm::abs(T[1] * R[0][0] - T[0] * R[1][0]) > ra + rb) return false;

		// A2 X B1
		ra = t_box_a.m_extents[0] * absR[1][1] + t_box_a.m_extents[1] * absR[0][1];
		rb = t_box_b.m_extents[0] * absR[2][2] + t_box_b.m_extents[2] * absR[2][0];
		if (glm::abs(T[1] * R[0][1] - T[0] * R[1][1]) > ra + rb) return false;

		// A2 X B2
		ra = t_box_a.m_extents[0] * absR[1][2] + t_box_a.m_extents[1] * absR[0][2];
		rb = t_box_b.m_extents[0] * absR[2][1] + t_box_b.m_extents[1] * absR[2][0];
		if (glm::abs(T[1] * R[0][2] - T[0] * R[1][2]) > ra + rb) return false;

		// All this for nothing
		return true;
	}
}


