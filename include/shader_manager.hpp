#ifndef _SHADER_MANAGER_HPP_
#define _SHADER_MANAGER_HPP_

#include <string>
#include <GL/glew.h>
#include <vector>

struct shader_info {
    GLenum type;
    const char* path;
    const char* name;
};

class shader_manager {
    private:
        std::string read_shader(const std::string& path);
        GLuint create_shader(GLenum type, const std::string& source, const std::string& name);
    public:
        GLuint create_program(const std::vector<shader_info>& sh);
        void delete_program(GLuint program);
};

static shader_manager s_shader_manager;

#endif