#include "ShaderLoader.hpp"
#include <fstream>
#include <iostream>

std::string ShaderLoader::readShader(std::string path) {
    std::string shaderCode;
	std::ifstream file(path, std::ios::in);

	if (!file.good()) {
		throw std::runtime_error("File " + path + " not found.");
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

GLuint ShaderLoader::createShader(GLenum type, std::string source, std::string name) {
    GLint result = 0;
    GLuint shader = glCreateShader(type);
    const GLchar* pShaderCode = source.c_str();
    const GLint codeSize = source.size();

    glShaderSource(shader, 1, &pShaderCode, &codeSize);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        GLint infoLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
        std::string shaderLog;
        shaderLog.resize(infoLength);
        glGetShaderInfoLog(shader, infoLength, nullptr, shaderLog.data());
        throw std::runtime_error("Error compiling shader: " + name + shaderLog);
    }

    return shader;
}

GLuint ShaderLoader::createProgram(std::string vShaderPath, std::string fShaderPath) {

    GLuint program = glCreateProgram();
    GLint result = 0;

    GLuint vertexShader = createShader(GL_VERTEX_SHADER, readShader(vShaderPath), "vertex shader");

    glAttachShader(program, vertexShader);
    
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, readShader(fShaderPath), "fragment shader");

    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (result == GL_FALSE) {
        GLint infoLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        std::string programLog;
        programLog.resize(infoLength);
        glGetProgramInfoLog(program, infoLength, nullptr, programLog.data());
        throw std::runtime_error("Error linking shaders: " + programLog);
    }

    return program;
}

void ShaderLoader::deleteProgram(GLuint program) {
    glDeleteProgram(program);
}