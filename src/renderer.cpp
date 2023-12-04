#include "renderer.hpp"

#include <glm/ext.hpp>

static inline void pass_matrices(GLuint program, glm::mat4 model, glm::mat4 cam, glm::mat4 per) {
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, (float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "camera_matrix"), 1, GL_FALSE, (float*)&cam);
	glUniformMatrix4fv(glGetUniformLocation(program, "perspective_matrix"), 1, GL_FALSE, (float*)&per);
}

namespace SE {
void renderer::render(const std::vector<rigid_body*>& bodies, const std::vector<light_source>& light_sources, float time) {


    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 camera_matrix = m_cam->create_camera_matrix();
    glm::mat4 perspective_matrix = m_cam->create_perspective_matrix();
	glm::mat4 skybox_translation = glm::translate(glm::mat4(1.), m_cam->m_pos);

    // skybox
    glUseProgram(m_skybox_program);
	pass_matrices(m_skybox_program, skybox_translation, camera_matrix, perspective_matrix);
    glUniform1i(glGetUniformLocation(m_skybox_program, m_skybox.uniform_name), 0);
    glActiveTexture(1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.id);
    glBindVertexArray(m_skybox_model.vertex_array);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, m_skybox_model.size, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);



    for (auto& body : bodies) {
        glUseProgram(body->m_program);

        // texture binding
        int tex_num = 0;
        for (auto& tex_info : body->m_textures) {
            glUniform1i(glGetUniformLocation(body->m_program, tex_info.uniform_name), tex_num);
            glActiveTexture(GL_TEXTURE0 + tex_num++);
            glBindTexture(GL_TEXTURE_2D, tex_info.id);
        }

        // light sources
        for (auto& ls : light_sources) {
            glUniform3fv(glGetUniformLocation(body->m_program, ls.uniform_pos_name), 1, (float*)&ls.pos);
            glUniform3fv(glGetUniformLocation(body->m_program, ls.uniform_color_name), 1, (float*)&ls.color);
        }

        glm::mat4 model = body->get_position(time);

		pass_matrices(body->m_program, model, camera_matrix, perspective_matrix);
        glUniform3fv(glGetUniformLocation(body->m_program, "camera_pos"), 1, (float*)&m_cam->m_pos);

        glBindVertexArray(body->m_context.vertex_array);
        glDrawElements(GL_TRIANGLES, body->m_context.size, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    glUseProgram(0);
}

}
