#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_
#include <vector>
#include <GL/glew.h>
#include "rigid_body.hpp"
#include "camera.hpp"
#include "texture_manager.hpp"
#include "mesh_manager.hpp"

namespace SE {

struct light_source {
    glm::vec3 pos;
    const char* uniform_pos_name;
    glm::vec3 color;
    const char* uniform_color_name;
};

class renderer {
    private:
        const camera* m_cam;
        texture_info m_skybox;
        GLuint m_skybox_program;
        render_context m_skybox_model;
    public:
        void set_active_camera(const camera& c) {
            m_cam = &c;
        }
        void render(const std::vector<rigid_body*>& bodies, const std::vector<light_source>& light_sources, float time); // time is needed for object positions
        void set_skybox(const texture_info& skybox, GLuint program, const render_context& model) { 
            m_skybox = skybox; 
            m_skybox_program = program; 
            m_skybox_model = model;
        }
};

static renderer s_renderer;
}


#endif