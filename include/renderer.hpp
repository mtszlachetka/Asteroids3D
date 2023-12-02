#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_
#include <vector>
#include <GL/glew.h>
#include "rigid_body.hpp"
#include "camera.hpp"

namespace SE {

struct light_source {
    glm::vec3 dir;
    const char* uniform_dir_name;
    glm::vec3 color;
    const char* uniform_color_name;
};

class renderer {
    private:
        const camera* m_cam;
    public:
        void set_active_camera(const camera& c) {
            m_cam = &c;
        }
        void render(const std::vector<rigid_body*>& bodies, const std::vector<light_source>& light_sources, float time); // time is needed for object positions
};

static renderer s_renderer;
}


#endif