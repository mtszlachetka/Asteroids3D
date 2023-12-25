#include <glm/gtx/euler_angles.hpp>
#include "camera.hpp"
#include <algorithm>
#include <iostream>
#include "input_module.hpp"

namespace se {

void input_module::tick() {
	if (m_p_active_window != nullptr) {
		if (glfwGetKey(m_p_active_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(m_p_active_window, true);
		}
		if (glfwGetKey(m_p_active_window, GLFW_KEY_W) == GLFW_PRESS) {
			notify(input_event::w_pressed);
		}
		if (glfwGetKey(m_p_active_window, GLFW_KEY_S) == GLFW_PRESS) {
			notify(input_event::s_pressed);
		}
		if (glfwGetKey(m_p_active_window, GLFW_KEY_A) == GLFW_PRESS) {
			notify(input_event::a_pressed);
		}
		if (glfwGetKey(m_p_active_window, GLFW_KEY_D) == GLFW_PRESS) {
			notify(input_event::d_pressed);
		}
		if (glfwGetKey(m_p_active_window, GLFW_KEY_Z) == GLFW_PRESS) {
			notify(input_event::z_pressed);
		}
		if (glfwGetKey(m_p_active_window, GLFW_KEY_X) == GLFW_PRESS) {
			notify(input_event::x_pressed);
		}
		if (glfwGetKey(m_p_active_window, GLFW_KEY_R) == GLFW_PRESS) {
			notify(input_event::r_pressed);
		}
		if (glfwGetKey(m_p_active_window, GLFW_KEY_F) == GLFW_PRESS) {
			notify(input_event::f_pressed);
		}
	}

}
}
