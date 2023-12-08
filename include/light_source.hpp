#ifndef _LIGHT_SOURCE_HPP_
#define _LIGHT_SOURCE_HPP_

#include <glm/glm.hpp>

namespace SE {

	class light_source {
		friend class renderer;
		using v3 = glm::vec3;
		private:
			v3 m_pos;
			const char* uniform_pos_name;
		public:
			light_source(const v3& t_pos, const char* t_pos_name) : m_pos(t_pos), uniform_pos_name(t_pos_name) {}
	};
}

#endif