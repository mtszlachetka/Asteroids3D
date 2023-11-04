#include "Renderer.hpp"
#include "Camera.hpp"

extern Camera camera;

void Renderer::renderBodies(GLuint program, std::vector<Body>& bodies) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);

    glm::mat4 cameraMatrix = camera.getCamera();

    for (Body& b : bodies) {
        glm::mat4 full = cameraMatrix * b.transformMatrix;
        glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&full);
        glUniform3fv(glGetUniformLocation(program, "color"), 1, (float*)&b.color);

        glBindVertexArray(b.context.vertexArray);
        glDrawElements(GL_TRIANGLES, b.context.size, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }

    glUseProgram(0);
}