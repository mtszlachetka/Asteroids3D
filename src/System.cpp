#include "System.hpp"
#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

extern float timeElapsed;
extern GLuint program;
extern Camera camera;

glm::mat4 System::position(float time) {
    return glm::mat4(1.0);
}


void System::render(const glm::mat4& parentMatrix) {
    /*
        The system calculates its tranformation matrix from distance and circulation time relative to its parent system.
        Then applies parent matrix for full transformation.
    */

    glm::mat4 modelMatrix = glm::mat4(1.);
    glm::mat4 translation = glm::translate(modelMatrix, displacement);
    glm::mat4 rotation = glm::rotate(modelMatrix, circTime == 0 ? 0 : timeElapsed / circTime, glm::vec3(1., 0., 0.));
    glm::mat4 scale = glm::scale(modelMatrix, glm::vec3(central.scale));

    glm::mat4 fullTransform =  parentMatrix * rotation * translation * scale;
    

    for (auto& d : dependants) {
        d.render(fullTransform);
    }

    glm::mat4 adjustedTransform = camera.getCamera() * fullTransform;
    
    glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&adjustedTransform);
    glUniform3fv(glGetUniformLocation(program, "color"), 1, (float*)&central.color);

    glBindVertexArray(central.context.vertexArray);
    glDrawElements(GL_TRIANGLES, central.context.size, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);


}