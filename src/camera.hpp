#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "rigid_body.hpp"
#include <glm/glm.hpp>
#include <iostream>

class camera {
    friend class io_processor;
    private:
        float m_near, m_far, m_aspect_ratio;
        glm::vec3 m_up, m_dir, m_side, m_pos;
    public:
        camera(float t_near, float t_far, glm::vec3 t_dir, glm::vec3 t_pos) : m_near(t_near), m_far(t_far), m_dir(t_dir), m_pos(t_pos) {
            rebase();
        }
        void follow(const rigid_body& rb);
        void rebase() {
            m_side = glm::normalize(glm::cross(m_dir, {0, 1, 0}));
            m_up = glm::normalize(glm::cross(m_side, m_dir));
        }
        void set_aspect_ratio(float r) { m_aspect_ratio = r; }
        ~camera() {}
        glm::mat4 create_camera_matrix() const;
        glm::mat4 create_perspective_matrix() const;
};

#endif