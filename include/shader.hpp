#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

namespace se {

	GLuint shader_from_string(GLenum t_type, const std::string_view& t_source);
	GLuint make_program(const std::vector<GLuint> t_shaders);

	inline void set_uniform_int(GLuint t_program, const std::string_view& t_name, int t_value) {
		glUniform1i(glGetUniformLocation(t_program, t_name.data()), t_value);
	}
	inline void set_uniform_float(GLuint t_program, const std::string_view& t_name, float t_value) {
		glUniform1f(glGetUniformLocation(t_program, t_name.data()), t_value);
	}
	inline void set_uniform_vec3(GLuint t_program, const std::string_view& t_name, const glm::vec3& t_value) {
		glUniform3fv(glGetUniformLocation(t_program, t_name.data()), 1, (float*)&t_value);
	}
	inline void set_uniform_mat4(GLuint t_program, const std::string_view& t_name, const glm::mat4& t_value) {
		glUniformMatrix4fv(glGetUniformLocation(t_program, t_name.data()), 1, GL_FALSE, (float*)&t_value);
	}
}


#endif