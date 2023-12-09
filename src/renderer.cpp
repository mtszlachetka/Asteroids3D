#include "renderer.hpp"

#include <glm/ext.hpp>
#include <iostream>

static inline void pass_matrices(GLuint program, glm::mat4 model, glm::mat4 cam, glm::mat4 per) {
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "camera_matrix"), 1, GL_FALSE, (float*)&cam);
	glUniformMatrix4fv(glGetUniformLocation(program, "perspective_matrix"), 1, GL_FALSE, (float*)&per);
}

namespace SE {

	void renderer::render(const GLFWwindow* window, const scene& sc) {
		float time_elapsed = static_cast<float>(glfwGetTime());
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const camera* cam = sc.m_cam;

		glm::mat4 camera_matrix = cam->get_camera_matrix();
		glm::mat4 perspective_matrix = cam->get_perspective_matrix();
		glm::mat4 skybox_pos = glm::translate(glm::mat4(1.), cam->m_pos);


		// skybox
		const skybox_info& skybox = sc.m_skybox;
		glUseProgram(skybox.program);
		pass_matrices(skybox.program, skybox_pos, camera_matrix, perspective_matrix);
		glUniform1i(glGetUniformLocation(skybox.program, skybox.cubemap.uniform_name), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap.id);
		glBindVertexArray(skybox.mesh.vertex_array);
		glDisable(GL_DEPTH_TEST);
		glDrawElements(GL_TRIANGLES, skybox.mesh.size, GL_UNSIGNED_INT, nullptr);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);

		for (auto& object : sc.m_objects) {
			GLuint program = object->m_program;
			glUseProgram(program);

			glUniform1f(glGetUniformLocation(program, "exposition"), sc.m_exposition);

			int tex_num = 0;
			for (auto& tex_info : object->m_textures) {
				glUniform1i(glGetUniformLocation(object->m_program, tex_info.uniform_name), tex_num);
				glActiveTexture(GL_TEXTURE0 + tex_num++);
				glBindTexture(GL_TEXTURE_2D, tex_info.id);
			}

			// light sources
			for (auto& ls : sc.m_light_sources) {
				glUniform3fv(glGetUniformLocation(object->m_program, ls.uniform_pos_name), 1, (float*)&ls.m_pos);
			}

			glm::mat4 model = object->get_position(time_elapsed);

			pass_matrices(object->m_program, model, camera_matrix, perspective_matrix);
			glUniform3fv(glGetUniformLocation(object->m_program, "camera_pos"), 1, (float*)&cam->m_pos);

			glBindVertexArray(object->m_mesh.vertex_array);
			glDrawElements(GL_TRIANGLES, object->m_mesh.size, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
		glUseProgram(0);
	}

}
