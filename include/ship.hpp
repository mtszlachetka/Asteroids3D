#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include "rigid_body.hpp"
#include <iostream>
#include <glm/ext.hpp>

namespace SE {

class ship : public rigid_body {
    using v3 = glm::vec3;
    friend class io_processor;
    private:
        float m_movespeed, m_anglespeed;
        glm::mat4 get_rotation_matrix() const {
            glm::mat4 rotation = {
                m_side.x, m_up.x, m_dir.x, 0,
                m_side.y, m_up.y, m_dir.y, 0,
                m_side.z, m_up.z, m_dir.z, 0,
                0, 0, 0, 1
            };
            return rotation;
        }
    public:
        ship(const SE::render_context& rc, GLuint program, const v3& pos, const v3& dir, 
                const std::vector<SE::texture_info>& tex, float mspeed, float aspeed) : rigid_body(rc, program, pos, dir, tex), 
                    m_movespeed(mspeed), m_anglespeed(aspeed) {}
        glm::mat4 get_position(float time) const { return glm::translate(glm::mat4(1.0), m_pos) * get_rotation_matrix() * glm::scale(glm::mat4(1.), glm::vec3(0.005)); }
        ~ship() {}
};
}


#endif