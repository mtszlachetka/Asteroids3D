#include "shader.hpp"
#include <fstream>
#include <iostream>

namespace se {


GLuint shader_from_string(GLenum t_type, const std::string_view &t_source) {
	GLuint shader = glCreateShader(t_type);

    const GLchar* p_shader_code = t_source.data();
    const GLint code_size = t_source.size();

    glShaderSource(shader, 1, &p_shader_code, &code_size);
    glCompileShader(shader);

    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint info_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
        std::string shader_log;
        shader_log.resize(info_length);
        glGetShaderInfoLog(shader, info_length, nullptr, shader_log.data());
        throw std::runtime_error("Error compiling shader: " + shader_log);
    }

    return shader;
}

GLuint make_program(const std::vector<GLuint> t_shaders) {
	GLuint program = glCreateProgram();

    for (GLuint shader: t_shaders) {
        glAttachShader(program, shader);
    }

    glLinkProgram(program);

	for (GLuint shader : t_shaders) {
        glDetachShader(program, shader);
    }

	GLint result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    
    if (result == GL_FALSE) {
        GLint info_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
        std::string program_log;
        program_log.resize(info_length);
        glGetProgramInfoLog(program, info_length, nullptr, program_log.data());
        throw std::runtime_error("Error linking shaders: " + program_log);
    }

    return program;
}
}