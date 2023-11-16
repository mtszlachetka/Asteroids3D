#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

extern unsigned width, height;

Camera::Camera(float near, float far, glm::vec3 dir, glm::vec3 pos) : n(near), f(far), cameraDir(dir), cameraPos(pos) {
    aspectRatio = width / height;
}

glm::mat4 Camera::createCameraMatrix() {
    cameraSide = glm::normalize(glm::cross(cameraDir, {0, 1, 0}));
    cameraUp = glm::normalize(glm::cross(cameraSide, cameraDir));

    glm::mat4 rotation = {
        cameraSide.x, cameraUp.x, -cameraDir.x, 0,
        cameraSide.y, cameraUp.y, -cameraDir.y, 0,
        cameraSide.z, cameraUp.z, -cameraDir.z, 0,
        0, 0, 0, 1
    };

    return rotation * glm::translate(glm::mat4(1.0), -cameraPos);
}

glm::mat4 Camera::createPerspectiveMatrix() {
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, (f + n) / (n - f), -1,
        0, 0, 2 * f * n / (n - f), 0
    };
}