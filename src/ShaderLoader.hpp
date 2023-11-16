#ifndef _SHADER_LOADER_HPP_
#define _SHADER_LOADER_HPP_

#include <string>
#include <GL/glew.h>

class ShaderLoader {
    private:
        std::string readShader(std::string path);
        GLuint createShader(GLenum type, std::string source, std::string name);
    public:
        GLuint createProgram(std::string vShaderPath, std::string fShaderPath);
        void deleteProgram(GLuint program);
};

static ShaderLoader gShaderLoader;

#endif