#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <array>
#include "texture.hpp"
#include "object.hpp"
#include <vector>

namespace se {

	class light_source {
		friend class scene;
		using v3 = glm::vec3;
		using v4 = glm::vec4;
		using m4 = glm::mat4;
		protected:
			float m_near, m_far;
			v4 m_light_vec;
			GLuint m_shadow_map_program;
			GLuint m_shadow_map_fbo;
			GLuint m_shadow_map;
			GLsizei SHADOW_MAP_WIDTH = 2048, SHADOW_MAP_HEIGHT = 2048;
		public:
			light_source(const v4& t_vec, float t_near, float t_far);
			void set_near(float n) { m_near = n; }
			void set_far(float f) { m_near = f; }
			virtual texture gen_shadow_map(const std::vector<object*>& objects) = 0;
			virtual ~light_source() = 0;
	};
	inline light_source::~light_source() {
		glDeleteTextures(1, &m_shadow_map);
		glDeleteFramebuffers(1, &m_shadow_map_fbo);
	}

	class punctual_light: public light_source {
		friend class scene;
		using v3 = glm::vec3;
		using m4 = glm::mat4;
		public:
			punctual_light(const v3& t_vec, float t_near, float t_far);
			virtual texture gen_shadow_map(const std::vector<object*>& objects);
			std::array<m4, 6> light_space_matrices();
			virtual ~punctual_light() {}
	};

	class directional_light: public light_source {
		friend class scene;
		using v3 = glm::vec3;
		using m4 = glm::mat4;
		private:
			float m_left, m_right, m_top, m_bottom;
		public:
			directional_light(const v3& t_vec, float t_near, float t_far, float t_left, float t_right, float t_top, float t_bottom);
			m4 light_space_matrix() { return glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far) * glm::lookAt(v3(m_light_vec), -v3(m_light_vec), v3(0, 1, 0)); }
			virtual texture gen_shadow_map(const std::vector<object*>& objects);
			virtual ~directional_light() {}
	};

}

#endif