#ifndef _SHADER_LOADER_HPP_
#define _SHADER_LOADER_HPP_

#include <string>
#include <GL/glew.h>

class ShaderLoader {
    private:
        static std::string readShader(std::string path);
        static GLuint createShader(GLenum type, std::string source, std::string name);
    public:
        static GLuint createProgram(std::string vShaderPath, std::string fShaderPath);
        static void deleteProgram(GLuint program);
};

#endif