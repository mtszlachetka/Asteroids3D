
#include <glm/gtx/euler_angles.hpp>
#include "io_processor.hpp"
#include "camera.hpp"

void io_processor::process_input(GLFWwindow* window, camera& cam) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.m_pos += cam.m_dir * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.m_pos -= cam.m_dir * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        cam.m_pos += cam.m_side * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        cam.m_pos -= cam.m_side * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        cam.m_pos += cam.m_up * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        cam.m_pos -= cam.m_up * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.m_dir = glm::vec3(glm::eulerAngleY(0.05f) * glm::vec4(cam.m_dir, 0));
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.m_dir = glm::vec3(glm::eulerAngleY(-0.05f) * glm::vec4(cam.m_dir, 0));
    }
	cam.rebase();
}


// void IOProcessor::processInput(GLFWwindow* window, Ship& ship) {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//         glfwSetWindowShouldClose(window, true);
//     }

//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//         ship.moveForward();
//     }

//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//         ship.moveBack();
//     }

//     if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
//         ship.moveRight();
//     }

//     if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
//         ship.moveLeft();
//     }

//     if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
//         ship.moveUp();
//     }

//     if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
//         ship.moveDown();
//     }

//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//         ship.turnLeft();
//     }

//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//         ship.turnRight();
//     }

//     if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
//         ship.rollLeft();
//     }

//     if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
//         ship.rollRight();
//     }


//     ship.talk();
// }