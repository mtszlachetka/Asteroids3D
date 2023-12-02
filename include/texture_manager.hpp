#ifndef _TEXTURE_MANAGER_HPP_
#define _TEXTURE_MANAGER_HPP_
#include <GL/glew.h>

namespace SE {
    struct texture_info {
        const char* uniform_name;
        GLuint id;
    };

    class texture_manager {
        public:
            texture_info load_texture(const char* path, const char* uniform_name);
            texture_info load_cubemap(const char** paths, const char* uniform_name);
    };

    static texture_manager s_texture_manager;
}


#endif