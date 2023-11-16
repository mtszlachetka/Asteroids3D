#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <glm/glm.hpp>

class Camera {
    friend class IOProcessor;
    friend class Renderer;
    friend class Ship;
    private:
        float n, f, aspectRatio;
        glm::vec3 cameraUp, cameraDir, cameraSide, cameraPos;
        glm::mat4 cameraMatrix;
    public:
        Camera(float near, float far, glm::vec3 dir, glm::vec3 pos);
        ~Camera() {}
        glm::mat4 createCameraMatrix();
        glm::mat4 createPerspectiveMatrix();
};

#endif