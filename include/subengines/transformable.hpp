#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

namespace se
{
	class transformable
	{
		using v3 = glm::vec3;
		using m4 = glm::mat4;
		using qu = glm::quat;

	protected:
		v3 m_position{0};
		qu m_orientation{1, 0, 0, 0};

	public:
		transformable() {}
		transformable(const v3 &t_position, const qu &t_orientation)
			: m_position(t_position), m_orientation(t_orientation) {}
		v3 get_position() const { return m_position; }
		qu get_orientation() const { return m_orientation; }
		void set_position(const v3 &t_position) { m_position = t_position; }
		void set_orientation(const qu &t_orientation) { m_orientation = t_orientation; }
		virtual ~transformable() {}
	};
}

#endif