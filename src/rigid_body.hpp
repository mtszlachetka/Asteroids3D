#ifndef _RIGID_BODY_HPP_
#define _RIGID_BODY_HPP_
#include "mesh_manager.hpp"
#include "texture_manager.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class rigid_body {
    using v3 = glm::vec3;
    friend class renderer;
    protected:
        render_context m_context;
        GLuint m_program;
        v3 m_color, m_pos, m_dir, m_side, m_up;
        std::vector<texture_info> m_textures;
    public:
        rigid_body(const render_context& rc, GLuint program, const v3& pos, const v3& dir, const v3& col, const std::vector<texture_info>& tex) : 
            m_context(rc), m_program(program), m_pos(pos), m_dir(dir), m_color(col), m_textures(tex) { rebase(); }
        void rebase() {
            m_side = glm::normalize(glm::cross(m_dir, {0, 1, 0}));
            m_up = glm::normalize(glm::cross(m_side, m_dir));
        }
        virtual glm::mat4 get_position(float time) const;
        virtual ~rigid_body() {}
};


#endif