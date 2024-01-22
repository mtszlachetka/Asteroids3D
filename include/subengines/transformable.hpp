#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

namespace se {
	class transformable {
		using v3 = glm::vec3;
		using m4 = glm::mat4;
		using qu = glm::quat;
		protected:
			v3 m_position {0};
			v3 m_scale {1,1,1};
			qu m_orientation {1,0,0,0};
		public:
			transformable() {}
			transformable(const v3& t_position, const v3& t_scale, const qu& t_orientation) 
				: m_position(t_position), m_scale(t_scale), m_orientation(t_orientation) {}
			v3 get_position() const { return m_position; }
			v3 get_scale() const { return m_scale; }
			qu get_orientation() const { return m_orientation; }
			void set_position(const v3& t_position) { m_position = t_position; }
			void set_scale(const v3& t_scale)  { m_scale = t_scale; }
			void set_orientation(const qu& t_orientation) { m_orientation = t_orientation; }
			virtual ~transformable() {}
	};
}

#endif