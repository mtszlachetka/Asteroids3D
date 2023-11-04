#include "System.hpp"
#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

extern GLuint program;
extern Camera camera;

std::vector<Body> System::calculatePositions(float time) {
    glm::mat4 modelMatrix = glm::mat4(1.);
    glm::mat4 translation = glm::translate(modelMatrix, displacement);
    glm::mat4 rotation = glm::rotate(modelMatrix, circTime == 0 ? 0 : time / circTime, glm::vec3(1., 0., 0.));
    glm::mat4 scale = glm::scale(modelMatrix, glm::vec3(central.scale));

    central.transformMatrix = rotation * translation;

    std::vector<Body> depBodies;
    for (auto& d : dependants) {
        std::vector<Body> newBodies = d.calculatePositions(time);
        depBodies.insert(depBodies.end(), newBodies.begin(), newBodies.end());
    }
    for (Body& b : depBodies) {
        b.transformMatrix = central.transformMatrix * b.transformMatrix;
    }
    central.transformMatrix = central.transformMatrix * scale;
    depBodies.push_back(central);
    return depBodies;
}
