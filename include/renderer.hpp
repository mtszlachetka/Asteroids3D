#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_
#include <vector>
#include <GL/glew.h>
#include "rigid_body.hpp"
#include "camera.hpp"

namespace SE {
class renderer {
    private:
        const camera* m_cam;
    public:
        void set_active_camera(const camera& c) {
            m_cam = &c;
        }
        void render(const std::vector<rigid_body*>& bodies, float time); // time is needed for object positions - needs better solution
};

static renderer s_renderer;
}


#endif