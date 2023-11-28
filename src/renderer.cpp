#include "renderer.hpp"

#include <glm/ext.hpp>
namespace SE {
void renderer::render(const std::vector<rigid_body*>& bodies, float time) {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 camera_matrix = m_cam->create_camera_matrix();
    glm::mat4 perspective_matrix = m_cam->create_perspective_matrix();

    for (auto& body : bodies) {
        glUseProgram(body->m_program);

        // texture binding
        int tex_num = 0;
        for (auto& tex_info : body->m_textures) {
            glUniform1i(glGetUniformLocation(body->m_program, tex_info.uniform_name), tex_num);
            glActiveTexture(GL_TEXTURE0 + tex_num++);
            glBindTexture(GL_TEXTURE_2D, tex_info.id);
        }

        glm::mat4 transform = perspective_matrix * camera_matrix * body->get_position(time);

        glUniformMatrix4fv(glGetUniformLocation(body->m_program, "transform"), 1, GL_FALSE, (float*)&transform);

        glBindVertexArray(body->m_context.vertex_array);
        glDrawElements(GL_TRIANGLES, body->m_context.size, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    glUseProgram(0);
}

}
