#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

#include <glm/glm.hpp>
#include <list>
#include <iostream>

#include "subengines/transformable.hpp"

namespace se {
	class rigid_body : virtual public transformable {
		using v3 = glm::vec3;
		protected:
			v3 m_velocity {0};
			float m_mass {0};
			// v3 m_inertia {0};
			// v3 m_angular_velocity {0};
		public:
			rigid_body();
			rigid_body(const v3& t_position, const v3& t_scale, const v3& t_velocity, float t_mass);
			v3 get_velocity() const { return m_velocity; }
			float get_mass() const { return m_mass; }
			void set_velocity(const v3& t_velocity) { m_velocity = t_velocity; }
			void set_mass(float t_mass) { m_mass = t_mass; }
			virtual ~rigid_body();
	};

	class physics_engine {
		private:
			std::list<rigid_body*> m_bodies;
			physics_engine() {}
		public:
			physics_engine(const physics_engine& other) = delete;
			physics_engine(const physics_engine&& other) = delete;
			void operator=(const physics_engine& other) = delete;
			void operator=(const physics_engine&& other) = delete;
			static physics_engine& get_instance() {
				static physics_engine instance;
				return instance;
			}
			void attach(rigid_body* rb) { m_bodies.push_back(rb); }
			void detach(rigid_body* rb) { m_bodies.remove(rb); }
			void tick();
	};
}

#endif