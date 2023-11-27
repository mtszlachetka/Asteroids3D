#include <glm/gtx/euler_angles.hpp>
#include "io_processor.hpp"
#include "camera.hpp"
#include <algorithm>
#include <iostream>

void io_processor::process_input(GLFWwindow* window, camera& cam, ship& sh) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        sh.m_pos += sh.m_dir * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        sh.m_pos -= sh.m_dir * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        sh.m_pos += sh.m_side * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        sh.m_pos -= sh.m_side * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        sh.m_pos += sh.m_up * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        sh.m_pos -= sh.m_up * 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        sh.m_dir = glm::vec3(glm::eulerAngleY(0.05f) * glm::vec4(sh.m_dir, 0));
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        sh.m_dir = glm::vec3(glm::eulerAngleY(-0.05f) * glm::vec4(sh.m_dir, 0));
    }

	double curr_x;
	double curr_y;
	glfwGetCursorPos(window, &curr_x, &curr_y);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	double x_offset = (curr_x - width / 2) * 0.05;
	double y_offset = (height / 2 - curr_y) * 0.05;


	m_yaw += x_offset;
	m_pitch += y_offset;

	m_pitch = std::clamp<double>(-89., m_pitch, 89.);
	
	// std::cout << "camera offset: x = " << x_offset << ", y = " << y_offset << "\n";

	// glm::vec3 front = {
	// 	cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
	// 	sin(glm::radians(m_pitch)),
	// 	sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
	// };

	// sh.m_dir = glm::normalize(front);

	sh.rebase();

	cam.m_pos = sh.m_pos - 0.3f * sh.m_dir + glm::vec3(0, 1, 0) * 0.1f;
	cam.m_dir = sh.m_dir;
	cam.rebase();
}