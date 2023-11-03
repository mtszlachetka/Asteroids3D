#include "IOProcessor.hpp"
#include "Camera.hpp"
#include <GLFW/glfw3.h>

void IOProcessor::processInput(GLFWwindow* window, Camera& cam) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.cameraPos += cam.cameraDir * moveSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.cameraPos -= cam.cameraDir * moveSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        cam.cameraPos += cam.cameraSide * moveSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        cam.cameraPos -= cam.cameraSide * moveSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        cam.cameraPos += cam.cameraUp * moveSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        cam.cameraPos -= cam.cameraUp * moveSpeed;
    }
}