#include "renderer.hpp"

#include <glm/ext.hpp>

void renderer::render(const std::vector<rigid_body*>& bodies) {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 camera_matrix = m_cam->create_camera_matrix();
    glm::mat4 perspective_matrix = m_cam->create_perspective_matrix();

    for (auto& body : bodies) {
        glUseProgram(body->m_program);
        glm::mat4 transform = perspective_matrix * camera_matrix * body->get_position(0);

        glUniformMatrix4fv(glGetUniformLocation(body->m_program, "transform"), 1, GL_FALSE, (float*)&transform);
        glUniform3fv(glGetUniformLocation(body->m_program, "color"), 1, (float*)&body->m_color);

        glBindVertexArray(body->m_context.vertex_array);
        glDrawElements(GL_TRIANGLES, body->m_context.size, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    glUseProgram(0);
}


