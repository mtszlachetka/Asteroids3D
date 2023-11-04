#include "Renderer.hpp"
#include "Camera.hpp"

#include <glm/ext.hpp>

extern Camera camera;

void Renderer::render(GLuint program, std::vector<Body>& bodies, Ship& ship) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);

    glm::mat4 cameraMatrix = camera.createCameraMatrix();
    glm::mat4 perspectiveMatrix = camera.createPerspectiveMatrix();

    for (Body& b : bodies) {
        glm::mat4 full = perspectiveMatrix * cameraMatrix * b.transformMatrix;
        glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&full);
        glUniform3fv(glGetUniformLocation(program, "color"), 1, (float*)&b.color);

        glBindVertexArray(b.context.vertexArray);
        glDrawElements(GL_TRIANGLES, b.context.size, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }
    
    cameraMatrix[0][2] = ship.shipDir.x;
    cameraMatrix[1][2] = ship.shipDir.y;
    cameraMatrix[2][2] = ship.shipDir.z;
    glm::mat4 shipMatrix = perspectiveMatrix * cameraMatrix * glm::translate(glm::mat4(1.), ship.shipPos);
    glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&shipMatrix);
    glUniform3fv(glGetUniformLocation(program, "color"), 1, (float*)&ship.color);
    glBindVertexArray(ship.context.vertexArray);
    glDrawElements(GL_TRIANGLES, ship.context.size, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);


    glUseProgram(0);
}
