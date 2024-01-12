#include "camera.hpp"
#include <glm/gtx/euler_angles.hpp>

namespace se {
	void free_camera::update(input_event e) {
		switch (e) {
			case input_event::w_pressed:
				m_pos += 0.05f * m_dir;
				break;
			case input_event::s_pressed:
				m_pos -= 0.05f * m_dir;
				break;
			case input_event::z_pressed:
				m_pos -= 0.05f * m_side;
				break;
			case input_event::x_pressed:
				m_pos += 0.05f * m_side;
				break;
			case input_event::r_pressed:
				m_pos += 0.05f * m_up;
				break;
			case input_event::f_pressed:
				m_pos -= 0.05f * m_up;
				break;
			case input_event::a_pressed:
				m_dir = glm::vec3(glm::eulerAngleY(0.05f) * glm::vec4(m_dir, 1.f));
				this->rebase();
				break;
			case input_event::d_pressed:
				m_dir = glm::vec3(glm::eulerAngleY(-0.05f) * glm::vec4(m_dir, 1.f));
				this->rebase();
				break;
		}
	}
}