#include "renderer.hpp"

#include <glm/ext.hpp>

void renderer::render_body(const rigid_body& body) {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 camera_matrix = m_cam->create_camera_matrix();
    glm::mat4 perspective_matrix = m_cam->create_perspective_matrix();

    glUseProgram(body.m_program);
    glm::mat4 transform = perspective_matrix * camera_matrix * body.get_position(0);

    glUniformMatrix4fv(glGetUniformLocation(body.m_program, "transform"), 1, GL_FALSE, (float*)&transform);
    glUniform3fv(glGetUniformLocation(body.m_program, "color"), 1, (float*)&body.m_color);

    glBindVertexArray(body.m_context.vertex_array);
    glDrawElements(GL_TRIANGLES, body.m_context.size, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glUseProgram(0);
}

// void Renderer::render(GLuint program, std::vector<Body>& bodies, Ship& ship) {
//     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     glUseProgram(program);

//     glm::mat4 cameraMatrix = camera.createCameraMatrix();
//     glm::mat4 perspectiveMatrix = camera.createPerspectiveMatrix();

//     for (Body& b : bodies) {
//         glm::mat4 full = perspectiveMatrix * cameraMatrix * b.transformMatrix;
//         glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&full);
//         glUniform3fv(glGetUniformLocation(program, "color"), 1, (float*)&b.color);

//         glBindVertexArray(b.context.vertexArray);
//         glDrawElements(GL_TRIANGLES, b.context.size, GL_UNSIGNED_INT, (void*)0);
//         glBindVertexArray(0);
//     }
    
//     glm::mat4 shipRotation = {
//         camera.cameraSide.x, camera.cameraSide.y, camera.cameraSide.z, 0,
//         camera.cameraUp.x, camera.cameraUp.y, camera.cameraUp.z, 0,
//         ship.shipDir.x, ship.shipDir.y, ship.shipDir.z, 0,
//         0, 0, 0, 1
//     };

//     glm::mat4 shipMatrix = perspectiveMatrix * cameraMatrix * glm::translate(glm::mat4(1.), ship.shipPos) * shipRotation * glm::scale(glm::mat4(1.), glm::vec3(0.3));
//     glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&shipMatrix);
//     glUniform3fv(glGetUniformLocation(program, "color"), 1, (float*)&ship.color);
//     glBindVertexArray(ship.context.vertexArray);
//     glDrawElements(GL_TRIANGLES, ship.context.size, GL_UNSIGNED_INT, (void*)0);
//     glBindVertexArray(0);


//     glUseProgram(0);
// }


