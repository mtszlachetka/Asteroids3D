#include <glm/gtx/euler_angles.hpp>
#include "io_processor.hpp"
#include "camera.hpp"
#include <algorithm>
#include <iostream>

namespace se {
void io_processor::process_input(GLFWwindow* window, controllable_object& sh) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        sh.m_pos += sh.m_dir * sh.m_movespeed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        sh.m_pos -= sh.m_dir * sh.m_movespeed;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        sh.m_pos += sh.m_side * sh.m_movespeed;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        sh.m_pos -= sh.m_side * sh.m_movespeed;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        sh.m_pos += sh.m_up * sh.m_movespeed;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        sh.m_pos -= sh.m_up * sh.m_movespeed;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        sh.m_dir = glm::vec3(glm::eulerAngleY(sh.m_anglespeed) * glm::vec4(sh.m_dir, 0));
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        sh.m_dir = glm::vec3(glm::eulerAngleY(-sh.m_anglespeed) * glm::vec4(sh.m_dir, 0));
    }


	sh.rebase();

	if (sh.m_camera != nullptr) {
		sh.m_camera->m_pos = sh.m_pos - 0.8f * sh.m_dir + glm::vec3(0, 1, 0) * 0.3f;
		sh.m_camera->m_dir = sh.m_dir;
		sh.m_camera->rebase();
	}
	
}
}

