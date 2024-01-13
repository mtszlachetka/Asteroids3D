#include "subengines/gameplay_engine.hpp"
#include "subengines/render_engine.hpp"
#include "camera.hpp"
#include "light_source.hpp"
#include "skybox.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "read_file.hpp"
#include "shader.hpp"
#include <memory>
#include <random>
#include "clock.hpp"

namespace se {
	void gameplay_engine::init() {
		// load assets
		m_player_mesh = se::load_model("../models/spaceship.obj");
		m_asteroid_mesh = se::load_model("../models/sphere.obj");
		m_station_mesh = se::load_model("../models/cube.obj");
		m_missile_mesh = se::load_model("../models/missile.obj");

		se::texture ship_diff = se::load_texture_2d_named("../textures/spaceshipPBR/diff.png", "diffuse_map");
		se::texture ship_normals = se::load_texture_2d_named("../textures/spaceshipPBR/norm.png", "normal_map");
		se::texture ship_amr = se::load_texture_2d_named("../textures/spaceshipPBR/amr.png", "amr_map");
		m_player_textures = {ship_diff, ship_normals, ship_amr};

		se::texture rock_diff = se::load_texture_2d_named("../textures/rock/diff.jpg", "diffuse_map");
		se::texture rock_normals = se::load_texture_2d_named("../textures/rock/norm.jpg", "normal_map");
		se::texture rock_amr = se::load_texture_2d_named("../textures/rock/arm.jpg", "amr_map");
		m_asteroid_textures = {rock_diff, rock_normals, rock_amr};

		se::texture missile_diff = se::load_texture_2d_named("../textures/missile/diff_.jpg", "diffuse_map");
		se::texture missile_normals = se::load_texture_2d_named("../textures/missile/norm_.jpg", "normal_map");
		se::texture missile_amr = se::load_texture_2d_named("../textures/missile/amr_.jpg", "amr_map");
		m_missile_textures = {missile_diff, missile_normals, missile_amr};

		GLuint vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/default.vert"));
		GLuint frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/default.frag"));
		m_program = se::make_program({vert, frag});


		m_station_textures = m_player_textures; // TODO: replace with proper station textures

		m_station_ptr = std::make_unique<se::station>(glm::vec3(0.f), glm::vec3(0.2f), m_station_mesh, m_station_textures, m_program, 1000);
		m_player_ptr = std::make_unique<se::player>(glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.2f), m_player_mesh, m_player_textures, m_program);


		m_last_spawn_time = 0.f;
	}
	void gameplay_engine::tick() {
		if (game_clock::get_instance().get_current_frame_time() - m_last_spawn_time > 5.f) {
			
			// TODO: move RNGs outside
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dist(15.0, 30.0);
			std::uniform_int_distribution<> minus(0,1);
			float x_pos = dist(gen);
			float y_pos = dist(gen);
			float z_pos = dist(gen);
			x_pos *= (minus(gen) == 0 ? 1 : -1);
			y_pos *= (minus(gen) == 0 ? 1 : -1);
			z_pos *= (minus(gen) == 0 ? 1 : -1);
			
			std::unique_ptr<se::asteroid> aptr = std::make_unique<se::asteroid>(
				glm::vec3(x_pos, y_pos, z_pos),
				glm::vec3(1.f),
				m_asteroid_mesh,
				m_asteroid_textures,
				m_program,
				glm::vec3(x_pos, y_pos, z_pos) * - 1.f / 25.f,
				1.f
			);
			m_asteroid_ptrs.push_back(std::move(aptr));
			m_last_spawn_time = game_clock::get_instance().get_current_frame_time();
		}
	}
	void gameplay_engine::spawn_missile() {
		if (game_clock::get_instance().get_current_frame_time() - m_last_shot_time < m_shooting_cooldown) return;
		std::unique_ptr<se::missile> mptr = std::make_unique<se::missile>(
			m_player_ptr->get_position(),
			glm::vec3(0.3f),
			m_player_ptr->get_direction(),
			m_player_ptr->get_side(),
			m_missile_mesh,
			m_missile_textures,
			m_program,
			m_player_ptr->get_direction() * 80.f,
			1
		);
		m_missile_ptrs.push_back(std::move(mptr));
		m_last_shot_time = game_clock::get_instance().get_current_frame_time();
	}

	void gameplay_engine::clear() { // this is needed to avoid a segfault after exiting main loop
		for (auto& a : m_asteroid_ptrs) {
			a.reset();
		}
		for (auto& m : m_missile_ptrs) {
			m.reset();
		}
	}
}