#include "gameplay/player.hpp"
#include "camera.hpp"
#include <glm/gtx/euler_angles.hpp>
#include "subengines/gameplay_engine.hpp"
#include "subengines/collision_engine.hpp"

namespace se {
	player::player(
		const v3& t_position,
		const v3& t_scale,
		const mesh& t_mesh,
		const std::list<texture>& t_textures,
		GLuint t_program
	) : transformable(t_position, t_scale),
		renderable(t_position, t_scale, t_mesh, t_textures, t_program),
		input_listener() {
		
		m_dir = v3(0.f, 0.f, 1.f);
		m_up = v3(0.f, 1.f, 0.f);
		m_side = v3(1.f, 0.f, 0.f);
		m_camera = std::make_unique<player_follow_camera>(0.01f, 20000.f, m_dir, v3(0.f));
		this->adjust_camera();

		render_engine::get_instance().set_camera(m_camera.get());

		collision_engine::get_instance().attach(this);
	}

	player::~player() {
		collision_engine::get_instance().detach_player();
	}

	void player::update(input_event e) {
		switch (e) {
			case input_event::w_pressed:
				m_position += 0.25f * m_dir;
				break;
			case input_event::s_pressed:
				m_position -= 0.25f * m_dir;
				break;
			case input_event::z_pressed:
				m_position -= 0.25f * m_side;
				break;
			case input_event::x_pressed:
				m_position += 0.25f * m_side;
				break;
			case input_event::r_pressed:
				m_position += 0.25f * m_up;
				break;
			case input_event::f_pressed:
				m_position -= 0.25f * m_up;
				break;
			case input_event::a_pressed:
				m_dir = glm::vec3(glm::eulerAngleY(0.05f) * glm::vec4(m_dir, 1.f));
				this->rebase();
				break;
			case input_event::d_pressed:
				m_dir = glm::vec3(glm::eulerAngleY(-0.05f) * glm::vec4(m_dir, 1.f));
				this->rebase();
				break;
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
		x_offset *= mouse_sensitivity;
		y_offset *= mouse_sensitivity;

		pitch = y_offset;
		yaw += x_offset;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		m_dir = glm::normalize(front);

		this->rebase();
		this->adjust_camera();
	}
}