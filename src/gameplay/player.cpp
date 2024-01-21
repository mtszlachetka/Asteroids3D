#include "gameplay/player.hpp"
#include "camera.hpp"
#include <glm/gtx/euler_angles.hpp>
#include "subengines/gameplay_engine.hpp"
#include "subengines/collision_engine.hpp"
#include <iostream>

namespace se {
	player::player(
		const v3& t_position,
		const v3& t_scale,
		const qu& t_orientation,
		const mesh& t_mesh,
		const std::list<texture>& t_textures,
		GLuint t_program
	) : transformable(t_position, t_scale, t_orientation),
		renderable(t_position, t_scale, t_orientation, t_mesh, t_textures, t_program),
		input_listener() {

		m_camera = std::make_unique<player_follow_camera>(0.01f, 20000.f, v3(0,0,1), v3(0));
		this->adjust_camera();

		render_engine::get_instance().set_camera(m_camera.get());

		collision_engine::get_instance().attach(this);
	}

	player::~player() {
		collision_engine::get_instance().detach_player();
	}

	void player::update(input_event e) {
		switch (e) {
			case input_event::w_pressed: {
				v3 direction = glm::toMat4(m_orientation) * glm::vec4(0,0,1,0);
				m_position += 0.25f * direction;
				break;
			}
			case input_event::s_pressed: {
				v3 direction = glm::toMat4(m_orientation) * glm::vec4(0,0,1,0);
				m_position -= 0.25f * direction;
				break;
			}
			case input_event::z_pressed: {
				v3 side = glm::toMat4(m_orientation) * glm::vec4(1,0,0,0);
				m_position += 0.25f * side;
				break;
			}
			case input_event::x_pressed: {
				v3 side = glm::toMat4(m_orientation) * glm::vec4(1,0,0,0);
				m_position -= 0.25f * side;
				break;
			}
			case input_event::r_pressed: {
				v3 up = glm::toMat4(m_orientation) * glm::vec4(0,1,0,0);
				m_position += 0.25f * up;
				break;
			}
			case input_event::f_pressed: {
				v3 up = glm::toMat4(m_orientation) * glm::vec4(0,1,0,0);
				m_position -= 0.25f * up;
				break;
			}
			case input_event::q_pressed: {
				v3 direction = glm::toMat4(m_orientation) * glm::vec4(0,0,1,0);
				m_orientation = m_orientation * glm::angleAxis(-0.05f, direction);
				break;
			}
			case input_event::e_pressed: {
				v3 direction = glm::toMat4(m_orientation) * glm::vec4(0,0,1,0);
				m_orientation = m_orientation * glm::angleAxis(0.05f, direction);
				break;
			}
			case input_event::space_pressed:
				gameplay_engine::get_instance().spawn_missile();
				break;
			case input_event::left_mouse_button_pressed:
				gameplay_engine::get_instance().spawn_missile();
				break;
		}

		this->adjust_camera();

	}

	void player::update_mouse_offset(double x_offset, double y_offset) {
		qu y_rotation = glm::angleAxis(glm::radians(static_cast<float>(-x_offset)), v3(m_orientation * glm::vec4(0,1,0,0)));
		qu x_rotation = glm::angleAxis(glm::radians(static_cast<float>(-y_offset)), v3(m_orientation * glm::vec4(1,0,0,0)));


		m_orientation = glm::normalize(y_rotation * x_rotation * m_orientation);
		this->adjust_camera();

	}
}