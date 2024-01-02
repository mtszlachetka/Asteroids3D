#include "object.hpp"
#include <glm/gtx/euler_angles.hpp>

namespace se {
	void player::update(input_event e) {
		// Everything related to user input
		switch (e) {
			case input_event::w_pressed:
				m_pos += m_movespeed * m_dir;
				break;
			case input_event::s_pressed:
				m_pos -= m_movespeed * m_dir;
				break;
			case input_event::z_pressed:
				m_pos -= m_movespeed * m_side;
				break;
			case input_event::x_pressed:
				m_pos += m_movespeed * m_side;
				break;
			case input_event::r_pressed:
				m_pos += m_movespeed * m_up;
				break;
			case input_event::f_pressed:
				m_pos -= m_movespeed * m_up;
				break;
			case input_event::a_pressed:
				m_dir = glm::vec3(glm::eulerAngleY(m_anglespeed) * glm::vec4(m_dir, 1.f));
				this->rebase();
				break;
			case input_event::d_pressed:
				m_dir = glm::vec3(glm::eulerAngleY(-m_anglespeed) * glm::vec4(m_dir, 1.f));
				this->rebase();
				break;
			case input_event::space_pressed:
				se::object new_laser_beam(laser_mesh, m_program, {laser_texture_1, laser_texture_2, laser_texture_3}, m_pos, 0.01f);
				new_laser_beam.set_velocity(laser_speed * m_dir);
				m_laser_beams.push_back(new_laser_beam);
				break;
		}

		this->adjust_camera();
	}
}