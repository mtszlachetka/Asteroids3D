#include "scene.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <array>
#include <iostream>

extern int WINDOW_WIDTH, WINDOW_HEIGHT;

static inline void pass_matrices(GLuint program, glm::mat4 model, glm::mat4 cam, glm::mat4 per) {
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "camera_matrix"), 1, GL_FALSE, (float*)&cam);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection_matrix"), 1, GL_FALSE, (float*)&per);
}


namespace se {

	void scene::render() {
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 camera_matrix = m_cam->get_camera_matrix();
		glm::mat4 perspective_matrix = m_cam->get_perspective_matrix();

		glm::mat4 skybox_pos = glm::translate(glm::mat4(1.), m_cam->m_pos);
		
		glUseProgram(m_skybox.program);
		pass_matrices(m_skybox.program, skybox_pos, camera_matrix, perspective_matrix);
		glUniform1i(glGetUniformLocation(m_skybox.program, m_skybox.cubemap.uniform_name), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.cubemap.id);
		glBindVertexArray(m_skybox.mesh.vertex_array);
		glDisable(GL_DEPTH_TEST);
		glDrawElements(GL_TRIANGLES, m_skybox.mesh.size, GL_UNSIGNED_INT, nullptr);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);
				

		for (auto& o : m_objects) {

			glm::vec4 lv = m_light->m_light_vec;

			texture_info depth_map = m_light->gen_shadow_map(m_objects);
			GLuint program = o->m_program;
			glUseProgram(program);

			glUniform3f(glGetUniformLocation(program, "light_vec"), lv.x, lv.y, lv.z);
			glUniform1i(glGetUniformLocation(program, depth_map.uniform_name), 0);
			glActiveTexture(GL_TEXTURE0);

			if (lv.w == 0.f) { // directional light
				glm::mat4 lsm = dynamic_cast<directional_light*>(m_light)->light_space_matrix();
				glUniformMatrix4fv(glGetUniformLocation(program, "light_space_matrix"), 1, GL_FALSE, (float*)&lsm);
				glBindTexture(GL_TEXTURE_2D, depth_map.id);

			} else if (lv.w == 1.f) { // punctual light
				glBindTexture(GL_TEXTURE_CUBE_MAP, depth_map.id);
				glUniform1f(glGetUniformLocation(program, "far"), m_light->m_far);
			}
			

			glUniform1f(glGetUniformLocation(program, "exposition"), m_exposition);

			
			int tex_num = 1;
			for (auto& tex_info : o->m_textures) {
				glUniform1i(glGetUniformLocation(o->m_program, tex_info.uniform_name), tex_num);
				glActiveTexture(GL_TEXTURE0 + tex_num++);
				glBindTexture(GL_TEXTURE_2D, tex_info.id);
			}
			

			glm::mat4 model = o->get_model_matrix();

			pass_matrices(o->m_program, model, camera_matrix, perspective_matrix);
			glUniform3fv(glGetUniformLocation(o->m_program, "camera_pos"), 1, (float*)&m_cam->m_pos);

			glBindVertexArray(o->m_mesh.vertex_array);
			glDrawElements(GL_TRIANGLES, o->m_mesh.size, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
		glUseProgram(0);
	}
}
