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

	}

	player::~player() {
	}

	void player::update(input_event e) {
		switch (e) {
			case input_event::w_pressed: {
				v3 direction = glm::toMat4(m_orientation) * glm::vec4(0,0,1,0);
				m_position += m_movespeed * direction;
				break;
			}
			case input_event::s_pressed: {
				v3 direction = glm::toMat4(m_orientation) * glm::vec4(0,0,1,0);
				m_position -= m_movespeed * direction;
				break;
			}
			case input_event::z_pressed: {
				v3 side = glm::toMat4(m_orientation) * glm::vec4(1,0,0,0);
				m_position += m_movespeed * side;
				break;
			}
			case input_event::x_pressed: {
				v3 side = glm::toMat4(m_orientation) * glm::vec4(1,0,0,0);
				m_position -= m_movespeed * side;
				break;
			}
			case input_event::r_pressed: {
				v3 up = glm::toMat4(m_orientation) * glm::vec4(0,1,0,0);
				m_position += m_movespeed * up;
				break;
			}
			case input_event::f_pressed: {
				v3 up = glm::toMat4(m_orientation) * glm::vec4(0,1,0,0);
				m_position -= m_movespeed * up;
				break;
			}
			case input_event::q_pressed: {
				v3 direction = v3(glm::toMat4(m_orientation) * glm::vec4(0,0,1,0));
				m_orientation = glm::angleAxis(-m_anglespeed, direction) * m_orientation;
				break;
			}
			case input_event::e_pressed: {
				v3 direction = v3(glm::toMat4(m_orientation) * glm::vec4(0,0,1,0));
				m_orientation = glm::angleAxis(m_anglespeed, direction) * m_orientation;
				break;
			}
			case input_event::space_pressed:
				gameplay_engine::get_instance().spawn_missile();
				break;
			case input_event::left_mouse_button_pressed:
				gameplay_engine::get_instance().spawn_missile();
				break;
			case input_event::left_shift_pressed:
				m_movespeed = 0.60f;
				m_camera->shake = true;
				m_camera_zoom_factor = m_base_zoom + 0.2f;
				break;
			case input_event::left_shift_released:
				m_movespeed = m_base_speed;
				m_camera->shake = false;
				m_camera_zoom_factor = m_base_zoom;
				break;
			case input_event::mouse_moved:
				auto [x_offset, y_offset] = input_engine::get_instance().get_mouse_coords();
				update_mouse_offset(x_offset, y_offset);
				break;
		}

		this->adjust_camera();

	}

	void player::update_mouse_offset(double x_offset, double y_offset) {
		qu y_rotation = glm::angleAxis(glm::radians(static_cast<float>(-x_offset)), v3(glm::toMat4(m_orientation) * glm::vec4(0,1,0,0)));
		qu x_rotation = glm::angleAxis(glm::radians(static_cast<float>(-y_offset)), v3(glm::toMat4(m_orientation) * glm::vec4(1,0,0,0)));

		m_orientation = glm::normalize(y_rotation * x_rotation * m_orientation);
		this->adjust_camera();

	}
}