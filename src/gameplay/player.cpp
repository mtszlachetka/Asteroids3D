#include "gameplay/player.hpp"
#include "camera.hpp"
#include <glm/gtx/euler_angles.hpp>
#include "subengines/gameplay_engine.hpp"
#include "subengines/collision_engine.hpp"
#include <iostream>
#include <thread>
#include <algorithm>

namespace se {
	player::player(
		const v3& t_position,
		const qu& t_orientation,
		const mesh& t_mesh,
		const std::list<texture>& t_textures,
		GLuint t_program
	) : transformable(t_position, t_orientation),
		renderable(t_position, t_orientation, t_mesh, t_textures, t_program),
		input_listener() {

		m_camera = std::make_unique<player_follow_camera>(0.01f, 20000.f, v3(0,0,1), v3(0));
		this->adjust_camera();

		render_engine::get_instance().set_camera(m_camera.get());

	}

	player::~player() {
	}


	void player::update(input_event e) {
		if (m_controls_active == false) return;
		switch (e) {
			case input_event::w_pressed: {
				v3 direction = glm::toMat3(m_orientation)[2];
				m_position += m_movespeed * direction;
				break;
			}
			case input_event::q_pressed: {
				v3 direction = glm::toMat3(m_orientation)[2];
				m_orientation = glm::angleAxis(-m_anglespeed, direction) * m_orientation;
				break;
			}
			case input_event::e_pressed: {
				v3 direction = glm::toMat3(m_orientation)[2];
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
				if (m_boost <= 0) {
					this->turn_off_boost();
					this->m_boost_time_used = static_cast<float>(game_clock::get_instance().get_current_frame_time());
					break;
				}
				this->turn_on_boost();
				this->use_boost();
				break;
			case input_event::left_shift_released:
				this->turn_off_boost();
				break;
			case input_event::left_alt_pressed:
				this->evade();
				break;
			case input_event::left_ctlr_pressed:
				this->turn_around();
				break;
			case input_event::mouse_moved:
				auto [x_offset, y_offset] = input_engine::get_instance().get_mouse_coords();
				update_mouse_offset(x_offset, y_offset);
				break;	
		}

		this->adjust_camera();

	}

	inline void player::turn_on_boost() {
		m_movespeed = 0.60f;
		m_camera->shake = true;
		m_camera_zoom_factor = m_base_zoom + 0.2f;
		m_boost_active = true;
	}

	inline void player::turn_off_boost() {
		m_movespeed = m_base_speed;
		m_camera->shake = false;
		m_camera_zoom_factor = m_base_zoom;
		m_boost_active = false;
	}

	void player::update_mouse_offset(double x_offset, double y_offset) {
		qu y_rotation = glm::angleAxis(glm::radians(static_cast<float>(-x_offset)), glm::toMat3(m_orientation)[1]);		
		qu x_rotation =	glm::angleAxis(glm::radians(static_cast<float>(-y_offset)), glm::toMat3(m_orientation)[0]);

		// if applying rotation would cause the camera to flip, replace with unit
		x_rotation = std::abs(glm::dot(glm::toMat3(x_rotation * m_orientation)[2], v3(0.f, 1.f, 0.f))) <= 0.9 ?
			x_rotation :
			qu(1.f, 0.f, 0.f, 0.f);

		m_orientation = glm::normalize(y_rotation * x_rotation * m_orientation);
		this->adjust_camera();

	}

	static void evade_animation(player* pl) {
		pl->set_controls(false);
		game_clock& clock = game_clock::get_instance();
		float timestamp = clock.get_current_frame_time();

		glm::quat player_orientation = pl->get_orientation();
		glm::mat3 rotation = glm::toMat3(player_orientation);

		glm::vec3 player_dir = rotation[2];

		float mult = pl->is_boosting() ? 2.4f : 1.f;

		auto [x, y] = input_engine::get_instance().get_mouse_coords();
		float sign = x >= 0.f ? 1.f : -1.f;

		glm::vec3 initial_position = pl->get_position();
		glm::vec3 goal_position = initial_position + rotation * glm::vec3(-10.f * sign, 0.f, 18.f) * mult;

		glm::quat initial_orientation = player_orientation;
		glm::quat current_orientation = initial_orientation;

		glm::quat checkpoint1 = glm::angleAxis(glm::radians(sign * 120.f), player_dir) * initial_orientation;
		glm::quat checkpoint2 = glm::angleAxis(glm::radians(sign * 240.f), player_dir) * initial_orientation;
		glm::quat checkpoint3 = initial_orientation;

		float t = 0.f;

		glm::vec3 current_position = initial_position;

		while (t != 1.f) {
			current_position = (1 - t) * initial_position + t * goal_position;
			pl->set_position(current_position);
			if (t <= 0.33f) {
				current_orientation = glm::slerp(initial_orientation, checkpoint1, t * 3);
			} else if (t <= 0.66f) {
				current_orientation = glm::slerp(checkpoint1, checkpoint2, (t - 0.33f) * 3);
			} else {
				current_orientation = glm::slerp(checkpoint2, initial_orientation, (t - 0.66f) * 3);
			}
			pl->set_orientation(current_orientation);
			t = std::min<float>(clock.get_current_frame_time() - timestamp, 1.f);
			pl->adjust_camera();
		}
		pl->set_position(goal_position);
		pl->set_orientation(initial_orientation);

		pl->set_controls(true);
	}

	void player::evade() {
		std::thread animation_thread(evade_animation, this);
		animation_thread.detach();
	}

	static void turn_animation(player* pl) {
		static constexpr glm::mat4 bezier_base = {
			-1,	3,	-3,	1,
			3,	-6,	3,	0,
			-3,	3,	0,	0,
			1,	0,	0,	0
		};

		pl->set_controls(false);
		game_clock& clock = game_clock::get_instance();
		float timestamp = clock.get_current_frame_time();

		glm::quat player_orientation = pl->get_orientation();
		glm::mat3 rotation = glm::toMat3(player_orientation);

		glm::vec3 initial_position = pl->get_position();

		auto [x, y] = input_engine::get_instance().get_mouse_coords();
		float sign = x >= 0.f ? 1.f : -1.f;

		float mult = pl->is_boosting() ? 2.4f : 1.f;

		glm::vec3 P0 = initial_position;
		glm::vec3 P1 = initial_position + rotation * glm::vec3(- sign * 10.f, 0.f, 15.f) * mult;
		glm::vec3 P2 = initial_position + rotation * glm::vec3(- sign * 20.f, 0.f, 15.f) * mult;
		glm::vec3 P3 = initial_position + rotation * glm::vec3(- sign * 30.f, 0.f, 0.f) * mult;

		glm::mat4x3 bezier_full = glm::mat4x3(P0, P1, P2, P3) * bezier_base;



		glm::quat initial_orientation = player_orientation;
		glm::quat checkpoint1 = glm::angleAxis(glm::radians(sign * 90.f), rotation[2]) * glm::angleAxis(glm::radians(-90.f), rotation[0]) * initial_orientation;
		glm::quat goal_orientation = glm::angleAxis(glm::radians(-sign * 180.f), rotation[1]) * initial_orientation;;

		float t = 0.f;
		while (t != 1.f) {
			
			glm::vec3 new_position;
			glm::quat new_orientation;
	
			glm::vec4 T(t * t * t, t * t, t, 1);
			new_position = bezier_full * T;
			pl->set_position(new_position);

			// update orientation with slerp
			if (t <= 0.5f) {
				new_orientation = glm::slerp(initial_orientation, checkpoint1, t * 2.f);
			} else {
				new_orientation = glm::slerp(checkpoint1, goal_orientation, (t - 0.5f) * 2.f);
			}
			pl->set_orientation(new_orientation);
			t = std::min<float>((clock.get_current_frame_time() - timestamp) / 2.f, 1.f);
			pl->adjust_camera();
		}
		pl->set_position(bezier_full * glm::vec4(1.f));
		pl->set_orientation(goal_orientation);
		pl->set_controls(true);
	} 

	void player::turn_around() {
		std::thread animation_thread(turn_animation, this);
		animation_thread.detach();
	}

}