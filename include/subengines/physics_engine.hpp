#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

#include <glm/glm.hpp>
#include <list>
#include <iostream>

#include "subengines/transformable.hpp"

namespace se
{
	class rigid_body : virtual public transformable
	{
		using v3 = glm::vec3;
		using qu = glm::quat;

	protected:
		v3 m_velocity = v3(0.f);
		float m_mass = 0.f;
		v3 m_inertia = v3(1.f);
		v3 m_angular_velocity = v3(0.f);

	public:
		rigid_body(const v3 &t_position, const qu &t_orientation, const v3 &t_velocity, float t_mass, const v3 &t_inertia, const v3 &t_angular);
		v3 get_velocity() const { return m_velocity; }
		v3 get_inertia() const { return m_inertia; }
		v3 get_angular() const { return m_angular_velocity; }
		float get_mass() const { return m_mass; }
		void set_velocity(const v3 &t_velocity) { m_velocity = t_velocity; }
		void set_mass(float t_mass) { m_mass = t_mass; }
		virtual ~rigid_body();
	};

	class physics_engine
	{
	private:
		std::list<rigid_body *> m_bodies;
		physics_engine() {}

	public:
		physics_engine(const physics_engine &other) = delete;
		physics_engine(const physics_engine &&other) = delete;
		void operator=(const physics_engine &other) = delete;
		void operator=(const physics_engine &&other) = delete;
		static physics_engine &get_instance()
		{
			static physics_engine instance;
			return instance;
		}
		void attach(rigid_body *rb) { m_bodies.push_back(rb); }
		void detach(rigid_body *rb) { m_bodies.remove(rb); }
		void tick();
	};
}

#endif