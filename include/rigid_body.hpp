#ifndef _RIGID_BODY_HPP_
#define _RIGID_BODY_HPP_
#include "mesh_manager.hpp"
#include "texture_manager.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace SE {
class rigid_body {
    using v3 = glm::vec3;
    friend class renderer;
    protected:
        glm::mat4 (*m_position_callback)(float); // awful - temporary solution 
        render_context m_context;
        GLuint m_program;
        v3 m_pos, m_dir, m_side, m_up; // moving frame - usused for now, set to 0
        std::vector<SE::texture_info> m_textures;
    public:
        rigid_body(const SE::render_context& rc, GLuint program, const v3& pos, const v3& dir, const std::vector<SE::texture_info>& tex) : 
            m_context(rc), m_program(program), m_pos(pos), m_dir(dir), m_textures(tex) { 
                rebase(); 
                m_position_callback = [](float time) -> glm::mat4 { return glm::mat4(1.); };
            }
        void rebase() {
            m_side = glm::normalize(glm::cross(m_dir, {0, 1, 0}));
            m_up = glm::normalize(glm::cross(m_side, m_dir));
        }
        void set_position_callback(glm::mat4 (*callback)(float)) { m_position_callback = callback; } 
        virtual glm::mat4 get_position(float time) const { return m_position_callback(time); };
        virtual ~rigid_body() {}
};
}




#endif