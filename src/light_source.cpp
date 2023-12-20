#include "light_source.hpp"
#include <glm/ext.hpp>
#include <string>
#include "shader_manager.hpp"

extern int WINDOW_WIDTH, WINDOW_HEIGHT;

namespace SE {

	light_source::light_source(const v4& t_vec, float t_near, float t_far) : m_near(t_near), m_far(t_far), m_light_vec(t_vec) {
		glGenFramebuffers(1, &m_shadow_map_fbo);
		glGenTextures(1, &m_shadow_map);	
	}

	directional_light::directional_light(const v3& t_vec, float t_near, float t_far, float t_left, float t_right, float t_top, float t_bottom) : 
		light_source(glm::vec4(t_vec, 0.f), t_near, t_far), m_left(t_left), m_right(t_right), m_top(t_top), m_bottom(t_bottom) {

		m_shadow_map_program = s_shader_manager.create_program({
			s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/shadow.vert"),
			s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/shadow.frag")
		});

		glBindTexture(GL_TEXTURE_2D, m_shadow_map);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float border_color[] = {1.f, 1.f, 1.f, 1.f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

		glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadow_map, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	
	texture_info directional_light::gen_shadow_map(const std::vector<object*>& objects) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo);
		glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(m_shadow_map_program);
		m4 lsm = light_space_matrix();
		glUniformMatrix4fv(glGetUniformLocation(m_shadow_map_program, "light_space_matrix"), 1, GL_FALSE, (float*)&lsm);
		GLint current_cull;
		glCullFace(GL_FRONT);
		for (auto& o : objects) {
			if (o->m_transparent) continue;
			m4 model_matrix = o->get_model_matrix();
			glUniformMatrix4fv(glGetUniformLocation(m_shadow_map_program, "model_matrix"), 1, GL_FALSE, (float*)&model_matrix);
			glBindVertexArray(o->m_mesh.vertex_array);
			glDrawElements(GL_TRIANGLES, o->m_mesh.size, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
		glCullFace(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		return { "shadow_map", m_shadow_map };
	}

	punctual_light::punctual_light(const v3& t_vec, float t_near, float t_far) : 
		light_source(glm::vec4(t_vec, 1.f), t_near, t_far) {

		m_shadow_map_program = s_shader_manager.create_program({
			s_shader_manager.create_shader(GL_VERTEX_SHADER, "../shaders/cube_depth.vert"),
			s_shader_manager.create_shader(GL_GEOMETRY_SHADER, "../shaders/cube_depth.geo"),
			s_shader_manager.create_shader(GL_FRAGMENT_SHADER, "../shaders/cube_depth.frag")
		});
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadow_map);
		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		float border_color[] = {1.f, 1.f, 1.f, 1.f};
		glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, border_color);

		glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadow_map, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	texture_info punctual_light::gen_shadow_map(const std::vector<object*>& objects) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo);
		glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		glUseProgram(m_shadow_map_program);
		std::array<m4, 6> matrices = light_space_matrices();
		glUniform1f(glGetUniformLocation(m_shadow_map_program, "far"), m_far);
		glUniform3f(glGetUniformLocation(m_shadow_map_program, "light_vec"), m_light_vec.x, m_light_vec.y, m_light_vec.z);

		for (int i = 0; i < 6; i++) {
			glUniformMatrix4fv(glGetUniformLocation(m_shadow_map_program, ("transforms[" +std::to_string(i)+ "]").c_str()), 1, GL_FALSE, (float*)&(matrices[i]));
		}
		for (auto& o : objects) {
			if (o->m_transparent) continue;
			m4 model_matrix = o->get_model_matrix();
			glUniformMatrix4fv(glGetUniformLocation(m_shadow_map_program, "model_matrix"), 1, GL_FALSE, (float*)&model_matrix);
			

			glBindVertexArray(o->m_mesh.vertex_array);
			glDrawElements(GL_TRIANGLES, o->m_mesh.size, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}

		glCullFace(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		return { "shadow_map", m_shadow_map };
	}

	std::array<glm::mat4, 6> punctual_light::light_space_matrices() {
		float aspect = static_cast<float>(SHADOW_MAP_WIDTH) / static_cast<float>(SHADOW_MAP_HEIGHT);

		m4 projection = glm::perspective(glm::radians(90.f), aspect, m_near, m_far);

		std::array<m4, 6> transforms;

		static v3 dirs[6] = {
			v3(1, 0, 0),
			v3(-1, 0, 0),
			v3(0, 1, 0),
			v3(0, -1, 0),
			v3(0, 0, 1),
			v3(0, 0, -1)
		};

		static v3 ups[6] = {
			v3(0, -1, 0),
			v3(0, -1, 0),
			v3(0, 0, 1),
			v3(0, 0, -1),
			v3(0, -1, 0),
			v3(0, -1, 0)
		};

		for (int i = 0; i < 6; i++) {
			transforms[i] = projection * glm::lookAt(v3(m_light_vec), v3(m_light_vec) + dirs[i], ups[i]);
		}

		return transforms;
	}
}