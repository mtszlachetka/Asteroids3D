#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_
#include <vector>
#include <GL/glew.h>
#include "rigid_body.hpp"
#include "camera.hpp"

class renderer {
    private:
        const camera* m_cam;
    public:
        void render_body(const rigid_body& body);
        void set_active_camera(const camera& c) {
            m_cam = &c;
        }
        void render(const std::vector<rigid_body*>& bodies);
};

static renderer s_renderer;

#endif