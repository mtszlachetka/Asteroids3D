#include "subengines/render_engine.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shader.hpp"
#include <cstdint>
#include "read_file.hpp"
#include "debug.hpp"

extern int WINDOW_WIDTH, WINDOW_HEIGHT;

namespace se {
	renderable::renderable() : transformable() {
		render_engine::get_instance().attach(this);
	}
	renderable::renderable(const v3& t_position, const v3& t_scale, const qu& t_orientation, const se::mesh& t_mesh, const std::list<se::texture>& t_textures, GLuint t_program)
		: transformable(t_position, t_scale, t_orientation), m_mesh(t_mesh), m_textures(t_textures), m_program(t_program) {

		render_engine::get_instance().attach(this);
	}
	renderable::~renderable() {
		render_engine::get_instance().detach(this);
	}

	render_engine::render_engine() { // assuming shadow map is always being generated
		m_shadow_map_program = make_program({
			shader_from_string(GL_VERTEX_SHADER, read_file("../shaders/shadow.vert")),
			shader_from_string(GL_FRAGMENT_SHADER, read_file("../shaders/shadow.frag"))
		});
		glGenTextures(1, &m_shadow_map);
		glBindTexture(GL_TEXTURE_2D, m_shadow_map);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_shadow_map_width, m_shadow_map_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float border_color[] = {1.f, 1.f, 1.f, 1.f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

		glGenFramebuffers(1, &m_shadow_map_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadow_map, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void render_engine::gen_shadow_map() { // if multiple lights were to be supported, should take a pointer as argument
		if (m_light_source == nullptr) return;
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo);
		glViewport(0, 0, m_shadow_map_width, m_shadow_map_height);
		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(m_shadow_map_program);
		set_uniform_mat4(m_shadow_map_program, "light_space_matrix", m_light_space_matrix);
		glCullFace(GL_FRONT);
		for (const renderable* re : m_renderables) {
			set_uniform_mat4(m_shadow_map_program, "model_matrix", re->get_model_matrix());
			re->get_mesh().render();
		}
		glCullFace(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glUseProgram(0);
	}

	void render_engine::tick() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// if (m_skybox != nullptr && m_camera != nullptr) { assume always true
			glm::mat4 skybox_pos = glm::translate(glm::mat4(1.), m_camera->get_position());
			GLuint program = m_skybox->get_program();
			glUseProgram(program);
			set_uniform_mat4(program, "model_matrix", skybox_pos);
			set_uniform_mat4(program, "camera_matrix", m_camera->get_camera_matrix());
			set_uniform_mat4(program, "projection_matrix", m_camera->get_perspective_matrix());
			set_uniform_int(program, m_skybox->get_cubemap().m_name, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_skybox->get_cubemap().m_id);
			glDisable(GL_DEPTH_TEST);
			m_skybox->get_mesh().render();
			glEnable(GL_DEPTH_TEST);
			glUseProgram(0);
		// }

		gen_shadow_map();

		for (const renderable* re : m_renderables) {
			glUseProgram(re->get_program());
			set_uniform_mat4(re->get_program(), "model_matrix", re->get_model_matrix());
			// if (m_camera != nullptr) { assume always true
				set_uniform_mat4(re->get_program(), "camera_matrix", m_camera->get_camera_matrix());
				set_uniform_mat4(re->get_program(), "projection_matrix", m_camera->get_perspective_matrix());
				set_uniform_vec3(re->get_program(), "camera_pos", m_camera->get_position());
			// }
			// uint8_t tex_num = m_skybox != nullptr ? 1 : 0;
			uint8_t tex_num = 1;
			for (const auto& info : re->get_textures()) {
				set_uniform_int(re->get_program(), info.m_name, tex_num);
				glActiveTexture(GL_TEXTURE0 + tex_num++);
				glBindTexture(GL_TEXTURE_2D, info.m_id);
			}

			// if (m_light_source != nullptr) { assume always true
				set_uniform_vec3(re->get_program(), "light_dir", m_light_source->get_position());
				set_uniform_mat4(re->get_program(), "light_space_matrix", m_light_space_matrix);
				set_uniform_int(re->get_program(), "shadow_map", tex_num);
				glActiveTexture(GL_TEXTURE0 + tex_num++);
				glBindTexture(GL_TEXTURE_2D, m_shadow_map);
			// }

			re->get_mesh().render();
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);

			// debug::texture_drawer drawer;
			// drawer.draw(m_shadow_map);
		}

	}
}