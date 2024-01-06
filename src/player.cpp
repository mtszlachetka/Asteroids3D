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
				if (static_cast<float>(glfwGetTime()) - last_time_shot_a_laser < 0.25f) {
					break;
				}
				std::unique_ptr<se::laser_beam> newLaserBeam = std::make_unique<se::laser_beam>(laserBeamPointer, m_pos, m_dir, m_side, m_up);
				newLaserBeam->set_velocity(m_dir * laserBeamPointer->laser_speed);
				laser_beams.push_back(std::move(newLaserBeam));
				last_time_shot_a_laser = static_cast<float>(glfwGetTime());
				break;
		}

		this->adjust_camera();
	}
}