#include "object.hpp"
#include <glm/gtx/euler_angles.hpp>

namespace se {
	void player::update(input_event e) {
		// Everything related to user input
		if (e == input_event::w_pressed) {
			m_pos += m_movespeed * m_dir;
		} else if (e == input_event::s_pressed) {
			m_pos -= m_movespeed * m_dir;
		} else if (e == input_event::z_pressed) {
			m_pos -= m_movespeed * m_side;
		} else if (e == input_event::x_pressed) {
			m_pos += m_movespeed * m_side;
		} else if (e == input_event::r_pressed) {
			m_pos += m_movespeed * m_up;
		} else if (e == input_event::f_pressed) {
			m_pos -= m_movespeed * m_up;
		} else if (e == input_event::a_pressed) {
			m_dir = glm::vec3(glm::eulerAngleY(m_anglespeed) * glm::vec4(m_dir, 1.f));
			rebase();
		} else if (e == input_event::d_pressed) {
			m_dir = glm::vec3(glm::eulerAngleY(-m_anglespeed) * glm::vec4(m_dir, 1.f));
			rebase();
		}

		// adjust camera
		if (m_camera != nullptr) {
			m_camera->m_pos = m_pos - 0.8f * m_dir + glm::vec3(0, 1, 0) * 0.3f; // TODO - make this configurable
			m_camera->m_dir = m_dir;
			m_camera->m_side = m_side;
			m_camera->m_up = m_up;
		}
	}
}