#ifndef COLLISION_ENGINE_HPP
#define COLLISION_ENGINE_HPP

#include <list>
#include <utility>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "mesh.hpp"

namespace se {

	static glm::vec3 dop14_axes[7] {
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
		{1, 1, 1},
		{1, 1, -1},
		{1, -1, 1},
		{1, -1, -1}
	};

	struct dop14 {
		float min[7];
		float max[7];
	};

	struct bounding_sphere {
		glm::vec3 center;
		float radius;
		bounding_sphere operator*(float f) { return {center * f, radius * f}; }
	};

	inline bounding_sphere translate(const bounding_sphere& sp, const glm::vec3& v) {
		return {sp.center + v, sp.radius};
	}

	struct aabb {
		glm::vec3 center;
		float r[3];
	};

	struct collision_info { // base for polymorphic pointers
		virtual ~collision_info() {}
	}; 
	struct asteroid_collision_info : collision_info {
		glm::vec3 t_velocity;
		float t_mass;
		asteroid_collision_info(const glm::vec3 v, float m) : t_velocity(v), t_mass(m) {}
	};


	class collidable {
		public:
			collidable();
			virtual dop14 get_dop14() = 0;
			virtual bounding_sphere get_bounding_sphere() const = 0;
			virtual void collide_with(collidable* cl, collision_info* info) = 0;
			virtual std::shared_ptr<collision_info> get_collision_info() { return {}; } // override if object needs to pass something to other colliding objects
			virtual ~collidable();
	};

	
	class collision_engine {
		private:
			std::list<collidable*> m_collidables;
			collision_engine() {}
		public:
			collision_engine(const collision_engine& other) = delete;
			collision_engine(const collision_engine&& other) = delete;
			void operator=(const collision_engine& other) = delete;
			void operator=(const collision_engine&& other) = delete;
			static collision_engine& get_instance() {
				static collision_engine instance;
				return instance;
			}
			void attach(collidable* cl) { m_collidables.push_back(cl); }
			void detach(collidable* cl) { m_collidables.remove(cl); }
			void tick();
	};

	bounding_sphere compute_bounding_sphere(const std::vector<se::vertex>& t_vertices);
	dop14 compute_dop14(const std::vector<se::vertex>& t_vertices, const glm::mat4& t_transform); 
}

#endif