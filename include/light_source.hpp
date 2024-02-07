#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <array>
#include "texture.hpp"
#include <vector>

namespace se {
	class light_source { // models directional light only
		using v3 = glm::vec3;
		private:
			v3 m_position = v3(0.f);
		public:
			light_source(const v3& t_position) : m_position(t_position) {}
			v3 get_position() const { return m_position; }
	};

}

#endif