#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "mesh.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "input_module.hpp"
#include <memory>

extern float delta_time, time_elapsed, last_time;

namespace se {

class object {
	friend class scene;
	friend class light_source;
	friend class directional_light;
	friend class punctual_light;
	friend class collision_detector;
	protected:
		camera* m_camera;
		GLuint m_program;
		mesh m_mesh;
		std::vector<texture> m_textures;
		bool m_transparent;
		glm::vec3 m_pos, m_velocity;
		float m_scale_factor; // scale factor determines both size and mass
		float m_mass;
		void update_position() { m_pos += m_velocity * delta_time; }
	public:
		object() = delete;
		object(const mesh& t_mesh, GLuint t_program, const std::vector<texture>& t_textures, const glm::vec3& t_pos, float t_scale, bool t_transparent = false) :
			m_mesh(t_mesh), m_program(t_program), m_textures(t_textures), m_transparent(t_transparent), m_pos(t_pos), m_scale_factor(t_scale), m_velocity(0), m_mass(t_scale) {}
		void set_velocity(const glm::vec3& t_v) { m_velocity = t_v; }
		virtual glm::mat4 get_model_matrix() { update_position(); return glm::translate(glm::mat4(1.f), m_pos) * glm::scale(glm::mat4(1), glm::vec3(m_scale_factor)); }
		virtual ~object() {}
		// virtual void render();
};

class laser_beam : public object {
		using v3 = glm::vec3;
		friend class player;
		private:
			float laser_speed;
			v3 m_dir, m_side, m_up;
			void update_position() { m_pos += m_velocity * delta_time; }
			glm::mat4 get_rotation_matrix() const {
				return {
					m_side.x, m_up.x, m_dir.x, 0,
					m_side.y, m_up.y, m_dir.y, 0,
					m_side.z, m_up.z, m_dir.z, 0,
					0, 0, 0, 1
				};
			}
		public:
			laser_beam(const se::mesh& t_mesh, GLuint t_program, const std::vector<se::texture>& tex, const v3& t_pos, float t_scale, float speed) :
					object(t_mesh, t_program, tex, t_pos, t_scale), laser_speed(speed) {}

			laser_beam(const laser_beam* existing_beam, const v3& player_pos, const v3& player_dir, const v3& player_side, const v3& player_up) :
					object(existing_beam->get_mesh(), existing_beam->get_program(), existing_beam->get_textures(), player_pos, existing_beam->get_scale_factor()),
					laser_speed(existing_beam->get_laser_speed()), m_dir(player_dir), m_side(player_side), m_up(player_up) {}
			
			virtual glm::mat4 get_model_matrix() override { update_position(); return glm::translate(glm::mat4(1.0), m_pos) * get_rotation_matrix() * glm::scale(glm::mat4(1), glm::vec3(m_scale_factor)); }

			const se::mesh& get_mesh() const {
				return m_mesh;
			}
			GLuint get_program() const {
				return m_program;
			}
			std::vector<se::texture> get_textures() const {
				return m_textures;
			}
			glm::vec3 get_position() const {
				return m_pos;
			}
			float get_scale_factor() const {
				return m_scale_factor;
			}
			float get_laser_speed() const {
				return laser_speed;
			}

			~laser_beam() {}
	};

class player : public object, public input_listener {
		using v3 = glm::vec3;
		private:
			v3 m_dir, m_side, m_up;
			float m_movespeed, m_anglespeed;
			const se::laser_beam* laserBeamPointer;
			std::vector<std::unique_ptr<se::laser_beam>> laser_beams;
			float last_time_shot_a_laser = 0.f;
			glm::mat4 get_rotation_matrix() const {
				return {
					m_side.x, m_up.x, m_dir.x, 0,
					m_side.y, m_up.y, m_dir.y, 0,
					m_side.z, m_up.z, m_dir.z, 0,
					0, 0, 0, 1
				};
			}
			void adjust_camera() {
				if (m_camera != nullptr) {
					m_camera->m_pos = m_pos - 0.8f * m_dir + glm::vec3(0, 1, 0) * 0.3f; // TODO - make this configurable
					m_camera->m_dir = m_dir;
					m_camera->m_side = m_side;
					m_camera->m_up = m_up;
				}
			}
		public:
			player(const se::mesh& t_mesh, GLuint t_program, const std::vector<se::texture>& tex, const v3& t_pos, float t_scale, const v3& t_dir, float mspeed, float aspeed, const se::laser_beam* laserBeamPtr) :
					object(t_mesh, t_program, tex, t_pos, t_scale),
					m_dir(t_dir), m_movespeed(mspeed), m_anglespeed(aspeed), laserBeamPointer(laserBeamPtr)
					{ rebase(); }
			virtual glm::mat4 get_model_matrix() override { return glm::translate(glm::mat4(1.0), m_pos) * get_rotation_matrix() * glm::scale(glm::mat4(1), glm::vec3(m_scale_factor)); }

			void attach_camera(camera& cam) { 
				m_camera = &cam;
				adjust_camera();
			}
			void rebase() {
				m_side = glm::normalize(glm::cross(m_dir, {0, 1, 0}));
				m_up = glm::normalize(glm::cross(m_side, m_dir));
			}
			void update(input_event event);
			~player() {}

			const std::vector<std::unique_ptr<se::laser_beam>>& get_laser_beams() const {
				return laser_beams;
			}
	};
}

#endif