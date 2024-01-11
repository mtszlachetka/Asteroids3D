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
#include "clock.hpp"

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
		void update_position() { game_clock& cl = game_clock::get_instance(); m_pos += m_velocity * cl.get_delta_time(); }
	public:
		object() = delete;
		object(const mesh& t_mesh, GLuint t_program, const std::vector<texture>& t_textures, const glm::vec3& t_pos, float t_scale, bool t_transparent = false) :
			m_mesh(t_mesh), m_program(t_program), m_textures(t_textures), m_transparent(t_transparent), m_pos(t_pos), m_scale_factor(t_scale), m_velocity(0), m_mass(t_scale) {}
		void set_velocity(const glm::vec3& t_v) { m_velocity = t_v; }
		virtual glm::mat4 get_model_matrix() { update_position(); return glm::translate(glm::mat4(1.f), m_pos) * glm::scale(glm::mat4(1), glm::vec3(m_scale_factor)); }
		virtual ~object() {}
		// virtual void render();
};

class missile : public object {
		using v3 = glm::vec3;
		friend class player;
		private:
			glm::mat4 rotationXMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 rotationZMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			float missile_speed;
			v3 m_dir, m_side, m_up;
			glm::mat4 get_rotation_matrix() const {
				glm::mat4 rotationMatrix = {
					m_side.x, m_up.x, m_dir.x, 0,
					m_side.y, m_up.y, m_dir.y, 0,
					m_side.z, m_up.z, m_dir.z, 0,
					0, 0, 0, 1
				};
				return rotationMatrix * rotationZMatrix * rotationXMatrix;
			}
		public:
			missile(const se::mesh& t_mesh, GLuint t_program, const std::vector<se::texture>& tex, const v3& t_pos, float t_scale, float speed) :
					object(t_mesh, t_program, tex, t_pos, t_scale), missile_speed(speed) {}

			missile(const missile* existing_missile, const v3& player_pos, const v3& player_dir, const v3& player_side, const v3& player_up) :
					object(existing_missile->get_mesh(), existing_missile->get_program(), existing_missile->get_textures(), player_pos, existing_missile->get_scale_factor()),
					missile_speed(existing_missile->get_missile_speed()), m_dir(player_dir), m_side(player_side), m_up(player_up) {}
			
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
			float get_missile_speed() const {
				return missile_speed;
			}

			~missile() {}
	};

class player : public object, public input_listener {
		using v3 = glm::vec3;
		private:
			v3 m_dir, m_side, m_up;
			float m_movespeed, m_anglespeed;
			float shooting_cooldown;
			const se::missile* missilePointer;
			std::vector<std::unique_ptr<se::missile>> missiles;
			float last_time_shot_a_missile = 0.f;
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
			player(const se::mesh& t_mesh, GLuint t_program, const std::vector<se::texture>& tex, const v3& t_pos, float t_scale, const v3& t_dir, float mspeed, float aspeed, const se::missile* missilePtr, float time_between_shots = 0.4f) :
					object(t_mesh, t_program, tex, t_pos, t_scale),
					m_dir(t_dir), m_movespeed(mspeed), m_anglespeed(aspeed), missilePointer(missilePtr), shooting_cooldown(time_between_shots)
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

			const std::vector<std::unique_ptr<se::missile>>& get_missiles() const {
				return missiles;
			}
	};
}

#endif