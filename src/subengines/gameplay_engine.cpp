#include "subengines/gameplay_engine.hpp"
#include "subengines/render_engine.hpp"
#include "subengines/collision_engine.hpp"
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
#include "subengines/debug.hpp"

namespace se {
	void gameplay_engine::init() {
		// load assets
		m_player_mesh = se::load_model("../models/spaceship.obj");
		m_asteroid_meshes[0] = se::load_model("../models/Asteroid_1_LOW_MODEL_.obj");
		m_asteroid_meshes[1] = se::load_model("../models/Asteroid_2_LOW_MODEL_.obj");
		m_asteroid_meshes[2] = se::load_model("../models/Asteroid_3_LOW_MODEL_.obj");
		m_asteroid_meshes[3] = se::load_model("../models/Asteroid_4_LOW_MODEL_.obj");
		m_station_mesh = se::load_model("../models/cube.obj");
		m_missile_mesh = se::load_model("../models/missile.obj");

		// scale meshes
		for (vertex& vert : m_player_mesh.m_vertices) {
			vert.m_position *= 0.2f;
		}

		for (vertex& vert : m_station_mesh.m_vertices) {
			vert.m_position *= 0.2f;
		}

		for (vertex& vert : m_missile_mesh.m_vertices) {
			vert.m_position *= 0.3f;
		}

		// compute bounding spheres
		m_player_sphere = compute_bounding_sphere(m_player_mesh.m_vertices);
		m_station_sphere = compute_bounding_sphere(m_station_mesh.m_vertices);
		m_missile_sphere = compute_bounding_sphere(m_missile_mesh.m_vertices);

		m_player_obb = compute_obb(m_player_mesh.m_vertices);
		m_station_obb = compute_obb(m_station_mesh.m_vertices);
		m_missile_obb = compute_obb(m_missile_mesh.m_vertices);

		for (int i = 0; i < 4; i++) {
			m_asteroid_spheres[i] = compute_bounding_sphere(m_asteroid_meshes[i].m_vertices);
			m_asteroid_obbs[i] = compute_obb(m_asteroid_meshes[i].m_vertices);
		}

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

		GLuint explosion_vert = se::shader_from_string(GL_VERTEX_SHADER, se::read_file("../shaders/explosion.vert"));
		GLuint explosion_geom = se::shader_from_string(GL_GEOMETRY_SHADER, se::read_file("../shaders/explosion.geom"));
		GLuint explosion_frag = se::shader_from_string(GL_FRAGMENT_SHADER, se::read_file("../shaders/explosion.frag"));
		m_explosion_program = se::make_program({explosion_vert, explosion_geom, explosion_frag});

		m_station_textures = m_player_textures; // TODO: replace with proper station textures

		m_station_ptr = std::make_unique<se::station>(glm::vec3(0.f), m_station_mesh, m_station_textures, m_program, m_station_sphere, m_station_obb, 5);
		m_player_ptr = std::make_unique<se::player>(glm::vec3(0.f, 0.f, 10.f), glm::quat(1,0,0,0), m_player_mesh, m_player_textures, m_program, m_player_sphere, m_player_obb);

	}

	void gameplay_engine::tick() {

		// check whether any objects should be destroyed
		m_asteroid_ptrs.remove_if([](std::unique_ptr<asteroid>& a) -> bool { return a->get_should_destruct(); });
		m_missile_ptrs.remove_if([](std::unique_ptr<missile>& m) -> bool { return m->get_should_destruct(); });
		if (m_station_ptr != nullptr && m_station_ptr->get_should_destruct()) m_station_ptr.reset();

		// spawn asteroids
		if (game_clock::get_instance().get_current_frame_time() - m_last_spawn_time > 2.f) { // every 2 seconds

			const glm::vec3& player_pos = m_player_ptr->get_position();
			const glm::vec3& player_dir = glm::toMat3(m_player_ptr->get_orientation())[2];

			
			static std::random_device rd;
			static std::mt19937 gen(rd());
			static std::uniform_real_distribution<> dist(15.0, 30.0);
			static std::uniform_int_distribution<> minus(0,1);
			static std::uniform_real_distribution<> sc(0.8, 2.0);
			static std::uniform_real_distribution<> vel(10.f, 25.f);
			static std::uniform_int_distribution<> mn(0,3);
			float x_pos = dist(gen);
			float y_pos = dist(gen);
			float z_pos = dist(gen);
			// always spawn behind player
			x_pos *= (x_pos * player_dir.x >= 0 ? -1 : 1);
			y_pos *= (y_pos * player_dir.y >= 0 ? -1 : 1);
			z_pos *= (z_pos * player_dir.z >= 0 ? -1 : 1);

			glm::vec3 final_pos = glm::vec3(x_pos, y_pos, z_pos) + player_pos;

			float scale_factor = sc(gen);
			float velocity_factor = vel(gen);

			int mesh_num = mn(gen);
			mesh scaled_mesh = m_asteroid_meshes[mesh_num];
			for (vertex& vert : scaled_mesh.m_vertices) {
				vert.m_position *= scale_factor;
			}

			float R = m_asteroid_spheres[mesh_num].radius * scale_factor;
			float I = 0.2f * scale_factor * R;
			
			std::unique_ptr<se::asteroid> aptr = std::make_unique<se::asteroid>(
				final_pos,
				glm::quat(1,0,0,0),
				scaled_mesh,
				m_asteroid_textures,
				m_explosion_program,
				final_pos * - 1.f / velocity_factor,
				scale_factor,
				glm::vec3(I),
				glm::vec3(vel(gen), vel(gen), vel(gen)),
				m_asteroid_spheres[mesh_num] * scale_factor,
				m_asteroid_obbs[mesh_num] * scale_factor
			);
			
			m_asteroid_ptrs.push_back(std::move(aptr));
			m_last_spawn_time = game_clock::get_instance().get_current_frame_time();
		}
	}
	void gameplay_engine::spawn_missile() {
		if (game_clock::get_instance().get_current_frame_time() - m_last_shot_time < m_shooting_cooldown) return;
		using v3 = glm::vec3;
		v3 player_dir = v3(glm::toMat4(m_player_ptr->get_orientation()) * glm::vec4(0,0,1,0));
		std::unique_ptr<se::missile> mptr = std::make_unique<se::missile>(
			m_player_ptr->get_position(),
			m_player_ptr->get_orientation() * glm::angleAxis(glm::radians(90.f), v3(1,0,0)),
			m_missile_mesh,
			m_missile_textures,
			m_program,
			player_dir * 80.f,
			1,
			glm::vec3(1.f),
			glm::vec3(0.f),
			m_missile_sphere,
			m_missile_obb
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
		m_player_ptr.reset();
		m_station_ptr.reset();
	}
}