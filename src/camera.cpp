#include "camera.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace SE {
    glm::mat4 camera::create_camera_matrix() const {

        glm::mat4 rotation = {
            m_side.x, m_up.x, -m_dir.x, 0,
            m_side.y, m_up.y, -m_dir.y, 0,
            m_side.z, m_up.z, -m_dir.z, 0,
            0, 0, 0, 1
        };

        return rotation * glm::translate(glm::mat4(1.0), -m_pos);
    }

    glm::mat4 camera::create_perspective_matrix() const {
        return {
            1, 0, 0, 0,
            0, m_aspect_ratio, 0, 0,
            0, 0, (m_far + m_near) / (m_near - m_far), -1,
            0, 0, 2 * m_far * m_near / (m_near - m_far), 0
        };
    }
}
