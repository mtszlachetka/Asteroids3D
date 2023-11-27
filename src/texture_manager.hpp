#ifndef _TEXTURE_MANAGER_HPP
#define _TEXTURE_MANAGER_HPP_
#include <GL/glew.h>

struct texture_info {
    const char* uniform_name;
    GLuint id;
};

class texture_manager {
    public:

};

static texture_manager s_texture_manager;

#endif