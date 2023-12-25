#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

extern float ASPECT_RATIO;

namespace se {

    class camera {
        friend class io_processor;
        friend class scene;
		friend class object;
        private:
            float m_near, m_far;
            glm::vec3 m_up, m_dir, m_side, m_pos;
        public:
            camera(float t_near, float t_far, glm::vec3 t_dir, glm::vec3 t_pos) : m_near(t_near), m_far(t_far), m_dir(t_dir), m_pos(t_pos) {
                rebase();
            }
            void rebase() {
                m_side = glm::normalize(glm::cross(m_dir, {0, 1, 0}));
                m_up = glm::normalize(glm::cross(m_side, m_dir));
            }

            glm::mat4 get_camera_matrix() const {

				return glm::mat4({
					m_side.x, m_up.x, -m_dir.x, 0,
					m_side.y, m_up.y, -m_dir.y, 0,
					m_side.z, m_up.z, -m_dir.z, 0,
					0, 0, 0, 1
				}) * glm::translate(glm::mat4(1.0), -m_pos);
			}
            glm::mat4 get_perspective_matrix() const {
				return {
					1, 0, 0, 0,
					0, ASPECT_RATIO, 0, 0,
					0, 0, (m_far + m_near) / (m_near - m_far), -1,
					0, 0, 2 * m_far * m_near / (m_near - m_far), 0
				};
			}

            ~camera() {}
    };
}


#endif