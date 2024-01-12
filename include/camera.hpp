#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

#include "subengines/input_engine.hpp"

extern float ASPECT_RATIO;

namespace se {

    class camera {
        protected:
            float m_near, m_far;
            glm::vec3 m_up, m_dir, m_side, m_pos;
        public:
            camera(float t_near, float t_far, const glm::vec3& t_dir, const glm::vec3& t_pos) : m_near(t_near), m_far(t_far), m_dir(t_dir), m_pos(t_pos) {
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
			glm::vec3 get_position() const { return m_pos; }
            ~camera() {}
    };

	class free_camera : public camera, public input_listener {
		public:
			free_camera(float t_near, float t_far, const glm::vec3& t_dir, const glm::vec3& t_pos) : camera(t_near, t_far, t_dir, t_pos), input_listener() {}
			virtual void update(input_event e) override;
	};
}


#endif