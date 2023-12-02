#include "renderer.hpp"

#include <glm/ext.hpp>
namespace SE {
void renderer::render(const std::vector<rigid_body*>& bodies, const std::vector<light_source>& light_sources, float time) {


    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 camera_matrix = m_cam->create_camera_matrix();
    glm::mat4 perspective_matrix = m_cam->create_perspective_matrix();

    // skybox
    glm::mat4 skybox_transform = glm::translate(perspective_matrix * camera_matrix, m_cam->m_pos);
    glUseProgram(m_skybox_program);
    glUniform1i(glGetUniformLocation(m_skybox_program, m_skybox.uniform_name), 0);
    glActiveTexture(1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.id);
    glUniformMatrix4fv(glGetUniformLocation(m_skybox_program, "transform"), 1, GL_FALSE, (float*)&skybox_transform);
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

        glm::mat4 transform = perspective_matrix * camera_matrix * model;

        glUniformMatrix4fv(glGetUniformLocation(body->m_program, "transform"), 1, GL_FALSE, (float*)&transform);
        glUniformMatrix4fv(glGetUniformLocation(body->m_program, "model"), 1, GL_FALSE, (float*)&model);
        glUniform3fv(glGetUniformLocation(body->m_program, "camera_pos"), 1, (float*)&m_cam->m_pos);

        glBindVertexArray(body->m_context.vertex_array);
        glDrawElements(GL_TRIANGLES, body->m_context.size, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    glUseProgram(0);
}

}
