#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include <GL/glew.h>
#include <texture.hpp>
#include <mesh.hpp>
#include <list>

#include "subengines/transformable.hpp"
#include "subengines/input_engine.hpp"
#include "camera.hpp"
#include "skybox.hpp"
#include "light_source.hpp"
#include "clock.hpp"

namespace se {
	class renderable : virtual public transformable {
		using v3 = glm::vec3;
		using m4 = glm::mat4;
		using qu = glm::quat;
		protected:
			se::mesh m_mesh;
			GLuint m_vao, m_vbo, m_ebo;
			std::list<se::texture> m_textures;
			GLuint m_program;
			bool m_occluder = true; // idicates whether object should cast shadow
			float m_time_of_destruction = 0.f;
		public:
			renderable();
			renderable(const v3& t_position, const qu& t_orientation, const se::mesh& t_mesh, const std::list<se::texture>& t_textures, GLuint t_program);
			se::mesh get_mesh() const { return m_mesh; }
			std::list<se::texture> get_textures() const { return m_textures; }
			GLuint get_program() const { return m_program; }
			virtual m4 get_model_matrix() const { return glm::translate(m4(1.f), m_position) * glm::toMat4(m_orientation); }
			float get_time_of_destruction() const { return m_time_of_destruction; }
			void set_time_of_destruction(float t_time) { m_time_of_destruction = t_time; }
			bool get_occluder() const { return m_occluder; }
			void render() const;
			virtual ~renderable();
	};

	class render_engine : public input_listener {
		private:
			std::list<const renderable*> m_renderables;
			const camera* m_camera = nullptr;
			const skybox* m_skybox = nullptr; // for now render engine holds a pointer to skybox - this can change later
			render_engine();
			GLuint m_shadow_map_fbo = 0;
			GLuint m_shadow_map_program = 0;
			GLuint m_shadow_map = 0;
			unsigned m_shadow_map_height = 2048;
			unsigned m_shadow_map_width = 2048;
			GLuint m_main_fbo = 0; // renderbuffer used for drawing objects
			GLuint m_main_color_buffers[2] {0}; // textures for bloom
			GLuint m_main_depth = 0; // depth attachment
			// rendering to quad spanning over screen
			GLuint m_banner_vao, m_banner_vbo, m_banner_ebo;
			GLuint m_hdr_program;
			static constexpr const GLfloat m_banner_vertices[] = {
				// vertex coords	// tex coords
				1.f,	1.f,	1.f,	0.f,
				1.f,	-1.f,	1.f,	1.f,
				-1.f,	1.f,	0.f,	0.f,	
				-1.f,	-1.f,	0.f,	1.f
			};
			static constexpr const GLuint m_banner_indices[] = {0, 1, 2, 1, 2, 3};
			const light_source* m_light_source = nullptr;
			GLuint m_blur_fbos[2] {0};
			GLuint m_blur_color_buffers[2] {0};
			GLuint m_blur_program = 0;
			void init_banner();
			void init_framebuffer();
			void init_shadow_map();
			void init_blur_buffers();
			void gen_shadow_map();
			void render_to_framebuffer();
			void render_to_screen();
			void apply_blur();
			glm::mat4 m_light_space_matrix = glm::mat4(1.f);
			float m_exposition = 1.f;
		public:
			render_engine(const render_engine& other) = delete;
			render_engine(const render_engine&& other) = delete;
			void operator=(const render_engine& other) = delete;
			void operator=(const render_engine&& other) = delete;
			static render_engine& get_instance() {
				static render_engine instance;
				return instance;
			}
			void attach(const renderable* rd) { m_renderables.push_back(rd); }
			void detach(const renderable* rd) { m_renderables.remove(rd); }
			void tick();
			void set_camera(const camera* t_camera) { m_camera = t_camera; }
			void set_skybox(const skybox* sk) { m_skybox = sk; }
			void set_light(const light_source* ls) { 
				m_light_source = ls; 
				m_light_space_matrix = glm::ortho(-30.f, 30.f, -30.f, 30.f, -60.f, 60.f) * glm::lookAt(m_light_source->get_position(), glm::vec3(0.f), glm::vec3(0, 1, 0)); 
			}
			void update(se::input_event e) override {
				if (e == input_event::j_pressed) {
					m_exposition -= 0.01f;
				}
				if (e == input_event::k_pressed) {
					m_exposition += 0.01f;
				}
			}
			void update_framebuffer();
	};
}

#endif