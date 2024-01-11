#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include <glm/glm.hpp>

namespace se {
	class rigid_body {
			using v3 = glm::vec3;
			protected:
				v3 m_velocity {0};
				float m_mass {0};
				// v3 m_inertia {0};
				// v3 m_angular_velocity {0};
			public:
				rigid_body() {}
				rigid_body(const v3& t_velocity, float t_mass) : m_velocity(t_velocity), m_mass(t_mass) {}
				v3 get_velocity() const { return m_velocity; }
				float get_mass() const { return m_mass; }
				void set_velocity(const v3& t_velocity) { m_velocity = t_velocity; }
				void set_mass(float t_mass) { m_mass = t_mass; }
		};
}
#endif